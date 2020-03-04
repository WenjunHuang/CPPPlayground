//
// Created by rick on 2020/2/29.
//

#include <future>
#include <experimental/coroutine>

std::future<int> compute_value() {
    int result = co_await std::async([]{
      return 30;

    });
    co_return result;
}
int main(){

}