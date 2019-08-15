#pragma once
#include <array>
#include <cassert>


template<typename T, auto MaxSize>class Stack {
public:
  using size_type = decltype(MaxSize);

private:
  std::array<T,MaxSize> elements;
  size_type numElements;

public:
  Stack();
  void push(const T& element);
  void pop();
  const T& top() const;
  bool empty() const{
    return numElements == 0;
  }
  size_type size() const{
    return numElements;
  }
};

template<typename T, auto MaxSize>Stack<T,MaxSize>::Stack():numElements(0){}

template<typename T, auto MaxSize>void Stack<T,MaxSize>::push(const T& element){
  assert(numElements < MaxSize);
  elements[numElements] = element;
  ++numElements;
};

template<typename T,auto MaxSize>
void Stack<T,MaxSize>::pop(){
  assert(!elements.empty());
  --numElements;
};

template<typename T,auto MaxSize>
const T& Stack<T,MaxSize>::top() const{
  assert(!elements.empty());
  return elements[numElements - 1];
};



