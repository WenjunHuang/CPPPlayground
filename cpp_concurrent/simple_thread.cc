//
// Created by HUANG WEN JUN on 2020/1/6.
//
#include <thread>
#include <iostream>
#include <vector>

void doWork(int id) {
    std::cout << "Hello " << id << '\n';
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0;i<20;++i) {
        threads.emplace_back(doWork,i);
    }

    for (auto& entry:threads)
        entry.join();
}