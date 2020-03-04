//
// Created by rick on 2020/2/29.
//
#include <experimental/coroutine>
#include <iostream>

struct suspend_always {
    bool await_ready() noexcept {
        return false;
    }

    void await_suspend(std::experimental::coroutine_handle<>) noexcept  {}

    void await_resume() noexcept {}
};

void my_coroutine() {
    std::cout << "my_coroutine about to suspend\n";
    co_await suspend_always{};
    std::cout << "my_coroutine was resumed\n";
}

int main() {
}