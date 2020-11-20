//
// Created by HUANG WEN JUN on 2020/11/18.
//
#include <algorithm>
#include <filesystem>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <rx.hpp>
#include <string>
#include <thread>
#include <vector>

using namespace std;

class FileNode;
class DirectoryNode;
class IFileFolderVisitor;

struct FileInformation {
    string name;
    long size;
};

class EntryNode {
  protected:
    string name;
    int isDir;
    long size;

  public:
    virtual bool IsDir()                          = 0;
    virtual long GetSize()                        = 0;
    virtual void Accept(IFileFolderVisitor& ivis) = 0;
    virtual ~EntryNode() {}
};

class IFileFolderVisitor {
  public:
    virtual void Visit(FileNode& fn)      = 0;
    virtual void Visit(DirectoryNode& dn) = 0;
};

class FileNode : public EntryNode {
  public:
    FileNode(string pname, long psize) {
        isDir = 0;
        name  = pname;
        size  = psize;
    }
    ~FileNode() {
        std::cout << "...Destructor FileNode ...." << name << std::endl;
    }

    virtual bool IsDir() { return isDir == 1; }
    virtual long GetSize() { return size; }
    virtual void Accept(IFileFolderVisitor& ivis) { ivis.Visit(*this); }
    string GetName() { return name; }
};

class DirectoryNode : public EntryNode {
    list<unique_ptr<EntryNode>> files;

  public:
    DirectoryNode(string pname) {
        files.clear();
        isDir = 1;
        name  = pname;
    }
    ~DirectoryNode() { files.clear(); }

    list<unique_ptr<EntryNode>>& GetAllFiles() { return files; }
    bool AddFile(string pname, long size) {
        files.push_back(unique_ptr<EntryNode>(new FileNode(pname, size)));
        return true;
    }
    bool AddDirectory(DirectoryNode* dn) {
        files.push_back(unique_ptr<EntryNode>(dn));
        return true;
    }
    virtual bool IsDir() override { return isDir == 1; }
    virtual long GetSize() override { return size; }
    virtual void Accept(IFileFolderVisitor& ivis) override {
        ivis.Visit(*this);
    }
    string GetName() { return name; }
};

class PrintFolderVisitor : public IFileFolderVisitor {
  public:
    void Visit(FileNode& fn) override {
        std::cout << fn.GetName() << std::endl;
    }
    void Visit(DirectoryNode& dn) override {
        std::cout << "In a directory " << dn.GetName() << std::endl;
        auto& ls = dn.GetAllFiles();
        for (auto& itr : ls) {
            itr->Accept(*this);
        }
    }
};

class DirHelper final {
  public:
    static DirectoryNode* SearchDirectory(const std::string& rootDir) {
        auto dirNode = new DirectoryNode{rootDir};

        std::filesystem::path path{rootDir};
        for (auto& p : std::filesystem::recursive_directory_iterator{path}) {
            if (p.is_directory())
                dirNode->AddDirectory(new DirectoryNode{p.path().string()});
            else
                dirNode->AddFile(p.path().string(), p.file_size());
        }
        return dirNode;
    }
};

void TestVisitor(string directory) {
    auto dirs = DirHelper::SearchDirectory(directory);
    if (dirs != nullptr) {
        auto fs = new PrintFolderVisitor();
        dirs->Accept(*fs);
    }
}

class FlattenVisitor : public IFileFolderVisitor {
    list<FileInformation> files;
    string currDir;

  public:
    FlattenVisitor() { currDir = ""; }
    ~FlattenVisitor() { files.clear(); }

    const list<FileInformation>& GetAllFiles() { return files; }

    void Visit(FileNode& fn) override {
        files.push_back(
            FileInformation{fn.GetName(), fn.GetSize()});
    }

    void Visit(DirectoryNode& dn) override {
        for (auto& itr : dn.GetAllFiles()) {
            itr->Accept(*this);
        }
    }
};

list<FileInformation> GetAllFiles(string dirname) {
    list<FileInformation> ret;
    auto dirs = DirHelper::SearchDirectory(dirname);
    if (dirs != nullptr) {
        FlattenVisitor visitor;
        dirs->Accept(visitor);
        ret = visitor.GetAllFiles();
    }
    return ret;
}

template <typename T> struct ActiveObject {
    rxcpp::subjects::subject<T> subj;
    void FireAndForget(T& item) { subj.get_subscriber().on_next(item); }

    rxcpp::observable<T> GetObservable() { return subj.get_observable(); }
};

class DirectoryEmitter {
    string rootDir;

    ActiveObject<FileInformation> act;

  public:
    DirectoryEmitter(string s) {
        rootDir = s;
        act.GetObservable().subscribe([](FileInformation item) {
            std::cout << item.name << ":" << item.size << std::endl;
        });
    }

    bool Trigger() {
        std::packaged_task<int()> task([&](){
            EmitDirEntry();return 1;
        });
        auto result = task.get_future();
        task();
        auto r = result.get();
        return true;
    }
    bool EmitDirEntry() {
        list<FileInformation> rs = GetAllFiles(rootDir);
        for (auto& a : rs) {
            act.FireAndForget(a);
        }
        return false;
    }
};
int main() {
    auto rs = GetAllFiles("C:\\Sources\\CPPPlayground");
    for (auto& as : rs)
        std::cout << as.name << std::endl;
}