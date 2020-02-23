//
// Created by rick on 2020/2/23.
//
#include <algorithm>
#include <boost/iterator/iterator_facade.hpp>
#include <cassert>

struct list_node{
    int data;
    list_node* next;
};
template <bool Const>
class list_of_ints_iterator
    : public boost::iterator_facade<list_of_ints_iterator<Const>,
                                    std::conditional_t<Const, const int, int>,
                                    std::forward_iterator_tag> {
    friend class boost::iterator_core_access;
    friend class list_of_ints;
    friend class list_of_ints_iterator<!Const>;

    using node_pointer = std::conditional_t<Const,const list_node*,list_node*>;
    node_pointer ptr_;

    explicit list_of_ints_iterator(node_pointer p):ptr_{p}{}

    auto& dereference() const {return ptr_->data;}
    void increment(){ptr_ = ptr_->next;}

    template<bool R>
    bool equal(const list_of_ints_iterator<R>& rhs) const {
        return ptr_ == rhs.ptr_;
    }

  public:
    operator list_of_ints_iterator<true>() const {
        return list_of_ints_iterator<true>{ptr_};
    }

};
class list_of_ints {
    list_node* head_ = nullptr;
    list_node* tail_ = nullptr;
    size_t size_     = 0;

  public:
    ~list_of_ints() {
        for (list_node *next, *p = head_; p != nullptr; p = next) {
            next = p->next;
            delete p;
        }
    }
    using iterator       = list_of_ints_iterator<false>;
    using const_iterator = list_of_ints_iterator<true>;
    iterator begin() { return iterator{head_}; }
    iterator end() { return iterator{nullptr}; }

    void push_back(int value) {
        list_node* new_tail = new list_node{value, nullptr};
        if (tail_) {
            tail_->next = new_tail;
        } else {
            head_ = new_tail;
        }
        tail_ = new_tail;
        size_ += 1;
    }
};



int main() {
    list_of_ints lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    int s = std::count_if(lst.begin(), lst.end(), [](int i) { return i >= 2; });
    assert(s == 2);

    int d = std::distance(lst.begin(),lst.end());
    assert(d == 3);
}