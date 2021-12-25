//
// Created by HUANG WEN JUN on 2021/12/25.
//

int int_identity(int x) {
    return x;
}

template<int X>
struct IntIdentity {
    static constexpr int value = X;
};

template<auto X>
struct ValueIdentity {
    static constexpr auto value = X;
};


// Sum
template<int X, int Y>
struct IntSum {
    static constexpr int value = X + Y;
};

template<auto X, auto Y>
struct Sum {
    static constexpr auto value = X + Y;
};

int main() {
    static_assert(42 == IntIdentity<42>::value);
    static_assert(42 == ValueIdentity<42>::value);
    static_assert(42ull == ValueIdentity<42ull>::value);
}