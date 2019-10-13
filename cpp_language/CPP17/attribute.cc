static void impl1() {}

[[maybe_unused]] static void impl2() {}

void foo() {
    int x                  = 42;
    [[maybe_unused]] int y = 42;
}