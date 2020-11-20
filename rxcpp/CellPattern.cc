//
// Created by rick on 2020/11/19.
//
#include <algorithm>
#include <map>
#include <memory>
#include <rx.hpp>
#include <string>

using namespace std;

class Cell {
  private:
    std::string name;
    std::map<std::string, Cell*> parents;
    std::unique_ptr<rxcpp::subjects::behavior<double>> behsubject;

  public:
    string GetName() const { return name; }
    void SetValue(double v) { behsubject->get_subscriber().on_next(v); }
    double GetValue() { return behsubject->get_value(); }

    rxcpp::observable<double> GetObservable() {
        return behsubject->get_observable();
    }

    bool GetCellNames(string& a,string& b){
        if (parents.size() != 2) {return false;}
        int i = 0;
        for (auto&p :parents){
            if (i == 0) a = p.first; else b = p.first;
            i++;
        }
        return true;
    }

    bool Recalculate() {
        string as,bs;
        if (!GetCellNames(as,bs)){
            return false;
        }
        auto a = parents[as];
        auto b = parents[bs];
        SetValue(a->GetValue() + b->GetValue());
        return true;
    }

    bool Attach(Cell& s){
        if(parents.size() >= 2) return false;
        parents.insert(std::make_pair(s.GetName(),&s));
        s.GetObservable()
        .subscribe([=](double a){
                Recalculate();
            });
        return true;
    }

    bool Detach(Cell& s){}

    Cell(std::string pname){
        name = pname;
        behsubject = std::make_unique<rxcpp::subjects::behavior<double>>(0);
    }
    ~Cell() {
        parents.clear();
    }
};

int main() { return 0; }