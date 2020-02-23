//
// Created by rick on 2020/2/23.
//
#include <cassert>
#include <iostream>
#include <iterator>
#include <type_traits>
struct list_node {
    int data;
    list_node* next;
};

template <bool Const> class list_of_ints_iterator {
    friend class list_of_ints;
    friend class list_of_ints_iterator<!Const>;
    using node_pointer =
        std::conditional_t<Const, const list_node*, list_node*>;

    node_pointer ptr_;

  public:
    // Member typedefes required by std::iterator_trait
    using difference_type = std::ptrdiff_t;
    using value_type      = int;
    using pointer   = std::conditional_t<Const, const int*, int*>;
    using reference = std::conditional_t<Const, const int&, int&>;
    using iterator_category = std::forward_iterator_tag;

    explicit list_of_ints_iterator(node_pointer ptr) : ptr_{ptr} {}
    reference operator*() const { return ptr_->data; }
    auto& operator++() {
        ptr_ = ptr_->next;
        return *this;
    }

    auto operator++(int) {
        auto it = *this;
        ++*(this);
        return it;
    }

    template <bool R>
    bool operator==(const list_of_ints_iterator<R>& rhs) const {
        return ptr_ == rhs.ptr_;
    }

    template <bool R>
    bool operator!=(const list_of_ints_iterator<R>& rhs) const {
        return ptr_ != rhs.ptr_;
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

template <typename Iterator, class Predicate>
auto count_if(Iterator begin, Iterator end, Predicate pred) {
    using Traits = std::iterator_traits<Iterator>;
    auto sum     = typename Traits::difference_type{};
    for (auto it = begin; it != end; ++it) {
        if (pred(*it)) {
            sum += 1;
        }
    }
    return sum;
}

template <typename Iterator> auto distance(Iterator begin, Iterator end) {
    using Traits = std::iterator_traits<Iterator>;
    if constexpr (std::is_base_of_v<std::random_access_iterator_tag,
                                    typename Traits::iterator_category>) {
        return (end - begin);
    } else {
        auto result = typename Traits::difference_type{};
        for (auto it = begin; it != end; ++it)
            ++result;
        return result;
    }
}

int main() {
    list_of_ints lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    int s = count_if(lst.begin(), lst.end(), [](int i) { return i >= 2; });
    assert(s == 2);

    int d = distance(lst.begin(),lst.end());
    assert(d == 3);
}