//
// Created by xxzyjy on 30/09/2017.
//

#ifndef CPPALGORITHM_DOUBLE_LINKED_LIST_H
#define CPPALGORITHM_DOUBLE_LINKED_LIST_H

#include <memory>

namespace alg {
  template<typename T>struct list_head {
    list_head<T> *next, *prev;
    T value;
  };

  template<typename T>class DoubleLinkedList {
  public:
    DoubleLinkedList(){}
  private:
    std::unique_ptr<list_head> _head;
  };



}

#endif //CPPALGORITHM_DOUBLE_LINKED_LIST_H
