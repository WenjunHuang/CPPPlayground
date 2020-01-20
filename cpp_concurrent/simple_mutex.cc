//
// Created by HUANG WEN JUN on 2020/1/7.
//
#include <list>
#include <mutex>
#include <algorithm>

std::list<int> someList;
std::mutex someMutex;

void addToList(int newValue) {
    std::lock_guard<std::mutex> guard(someMutex);
    someList.push_back(newValue);
}

bool listContains(int valueToFind) {
    std::lock_guard<std::mutex> guard(someMutex);
    return std::find(someList.begin(),someList.end(),valueToFind) != someList.end();
}

int main(){

}