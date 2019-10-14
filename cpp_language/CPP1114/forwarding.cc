#include <iostream>
class Widget{};
void process(const Widget& lvalArg) {
    std::cout << "lval"<<'\n';
}

void process(Widget&& rvalArg){
    std::cout <<"rval\n";
}

template<typename T>
void logAndProcess(T&& param) {
    process(std::forward<T>(param));
}

int main(){
    Widget w;
    logAndProcess(w);
    logAndProcess(std::move(w));
}