
using TNoexceptVoidFunc = void (*)() noexcept;
void SimpleNoexceptCall(TNoexceptVoidFunc func) {func();}

using TVoidFunc = void (*)();
void SimpleCall(TVoidFunc func){func();}

void fNoexcept() noexcept {}
void fRegular() {}

int main(){
    SimpleNoexceptCall(fNoexcept);

    SimpleCall(fNoexcept);
    SimpleCall(fRegular);
}