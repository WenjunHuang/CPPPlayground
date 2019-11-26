//
// Created by HUANG WEN JUN on 2019/11/26.
//
#include <folly/futures/Future.h>
#include <folly/executors/ThreadedExecutor.h>
#include <iostream>
#include <thread>

using namespace folly;
using namespace std;

void foo(int x) {
    cout<<"foo(" << x << ")" << endl;
}

int main(int argc,char *argv[]) {
    ThreadedExecutor executor;
    cout << "making Promise" << endl;

    auto mainThreadId = std::this_thread::get_id();
    cout << mainThreadId << endl;
    folly::makeFuture(10)
    .via(&executor)
    .thenValue([](int value){
      auto id = std::this_thread::get_id();
      cout << id << endl;
      foo(value);
    });
//    Promise<int> p;
//    Future<int> f = p.getSemiFuture().via(&executor);
//    auto f2 = move(f).thenValue(foo);
//    cout << "Future chain  made" << endl;


}