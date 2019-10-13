#include <array>
struct NonMoveable {
    NonMoveable(int x):v(x){}
    NonMoveable(const NonMoveable&) = delete;
    NonMoveable(NonMoveable&&) = delete;

    std::array<int,1024> arr;
    int v;
};

NonMoveable make(int val) {
    if (val > 0)
        return NonMoveable(val);
    return NonMoveable(-val);
}

int main() {
    auto largeNonMoveableObj = make(90);
    return largeNonMoveableObj.v;
}