//
// Created by rick on 2020/11/19.
//
#include <rx.hpp>
#include <cstdio>

using namespace std;

class ResourceLoan {
  private:
    FILE *file;
    string filename;
  public:
    ResourceLoan(const string& pfile):filename{pfile} {
        file =fopen(filename.c_str(),"rb");
    }

    int ReaBuffer(std::function<int(char pbuffer[],int val)> func) {
        if (file == nullptr)
            return -1;
        char buffer[1024];
        int result = fread(buffer,1,1024,file);
        return func(buffer,result);
    }
    ~ResourceLoan(){fclose(file);}
};

int main(){
    ResourceLoan res("C:\\Users\\rick\\java_error_in_idea.hprof");
    int nRead;
    auto reader = [](char *buffer,int val){
        std::cout << "Size " << val << std::endl;
        return val;
    };

    while((nRead = res.ReaBuffer(reader)) > 0){}
}