//
// Created by rick on 2021/12/2.
//
#include <iostream>
#define __cpp_lib_coroutine
#include <coroutine>
#include <exception>

template<typename CoroType>
struct CoroPromise {
    auto get_return_object() {
      std::cout << "- promise: get_return_object()\n";
      return std::coroutine_handle<CoroPromise<CoroType>>::from_promise(*this);
    }

    auto initial_suspend() {
      std::cout << "- promise: initial_suspend()\n";
      return std::suspend_always{};
    }

    auto final_suspend() noexcept {
      std::cout << "- promise: final_suspend()\n";
      return std::suspend_always{};
    }

    void unhandled_exception() {
      std::terminate();
    }

    void return_void() {
      std::cout << "- promise: return_void()\n";
    }
};

class CoroTask {
public:
    using promise_type = CoroPromise<CoroTask>;

private:
    std::coroutine_handle<promise_type> coroHdl;

public:
    CoroTask(auto h):coroHdl{h} {
      std::cout << "-CoroTask: construct\n";
    }
    ~CoroTask(){
      std::cout << "- CoroTask: destruct\n";
      if (coroHdl) {
        coroHdl.destroy();
      }
    }

    CoroTask(const CoroTask&) = delete;
    CoroTask& operator=(const CoroTask&) = delete;

    // API to resume the coroutine
    // - returns whether there is still something to process
    bool resume() const {
      std::cout << "- CoroTask: resume()\n";
      if (!coroHdl) {
        return false;
      }
      coroHdl.resume();
      return !coroHdl.done();
    }
};

int main() {

}