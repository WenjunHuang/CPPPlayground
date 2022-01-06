//
// Created by HUANG WEN JUN on 2021/12/18.
//
struct ListNode {
    int data;
    ListNode *next;
};

class ListOfIntsIterator {
    ListNode *ptr_;

    friend class ListOfInts;

    explicit ListOfIntsIterator(ListNode *p) : ptr_(p) {}

public:
    int &operator*() const { return ptr_->data; }

    ListOfIntsIterator &operator++() {
        ptr_ = ptr_->next;
        return *this;
    }

    ListOfIntsIterator operator++(int) {
        auto it = *this;
        ++*this;
        return it;
    }

    bool operator==(const ListOfIntsIterator &rhs) const {
        return ptr_ == rhs.ptr_;
    }

    bool operator!=(const ListOfIntsIterator &rhs) const {
        return ptr_ != rhs.ptr_;
    }
};

class ListOfInts {
    ListNode *head_ = nullptr;
    ListNode *tail_ = nullptr;
public:
    using iterator = ListOfIntsIterator;

    iterator begin() { return iterator{head_}; }

    iterator end() { return iterator{nullptr}; }
};

template<class Container, class Predicate>
int count_if(Container &ctr, Predicate pred) {
    int sum = 0;
    for (auto it = ctr.begin(); it != ctr.end(); ++it) {
        if (pred(*it)) {
            sum += 1;
        }
    }
    return sum;
}

int main() {
    int i = 0;
}
