//
// Created by rick on 19-7-21.
//
#include <deque>
#include <vector>
#include <iostream>

template <typename T, template <typename Elem, typename = std::allocator<Elem>>
                      class Cont = std::deque>
class Stack {
  private:
    Cont<T> elems;

  public:
    void push(T const&);
    void pop();
    T const& top() const;
    bool empty() const { return elems.empty(); }

    template <typename T2,
              template <typename Elem2, typename = std::allocator<Elem2>>
              class Cont2>
    Stack<T, Cont>& operator=(Stack<T2, Cont2> const&);

    friend std::ostream& operator<<(std::ostream& out,Stack& stack) {
        for (auto& elem: stack.elems) {
            out << elem << " ";
        }
        return out;
    }

    template <typename, template <typename, typename> class> friend class Stack;
};


template <typename T, template <typename, typename> class Cont>
void Stack<T, Cont>::push(const T& elem) {
    elems.push_back(elem);
}
template <typename T, template<typename,typename>class Cont> void Stack<T, Cont>::pop() {
    assert(!elems.empty());
    elems.pop_back();
}
template <typename T, template<typename,typename>class Cont> T const& Stack<T, Cont>::top() const {
    assert(!elems.empty());
    return elems.back();
}
template <typename T, template<typename,typename>class Cont>
template <typename T2, template<typename,typename>class Cont2>
Stack<T, Cont>& Stack<T, Cont>::operator=(const Stack<T2, Cont2>&op2) {
    elems.clear();
    elems.insert(elems.begin(),
        op2.elems.begin(),
        op2.elems.end());
    return *this;
}

int main() {
    Stack<int> myStack;
    myStack.push(10);
    myStack.push(200);

    Stack<float,std::vector>  myFloatStack;
    myFloatStack.push(20.0f);
    myFloatStack.push(391.1f);

    myStack = myFloatStack;

    std::cout << myStack << std::endl;
    std::cout << myFloatStack << std::endl;
}
