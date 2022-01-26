//
// Created by HUANG WEN JUN on 2021/12/18.
//
#include <type_traits>
#include <iterator>

struct ListNode {
    int data;
    ListNode *next;
};

template<bool Const>
class ListOfIntsIterator {
    friend class ListOfInts;

    friend class ListOfIntsIterator<!Const>;

    using node_pointer = std::conditional_t<Const, ListNode const *, ListNode *>;

    node_pointer ptr_;


    explicit ListOfIntsIterator(node_pointer p) : ptr_(p) {}

public:
    // Member typedefs required by std::iterator_traits
    using difference_type = std::ptrdiff_t;
    using value_type = int;
    using pointer = std::conditional_t<Const,int const*,int*>;
    using reference = std::conditional_t<Const,int const&,int &>;
    using iterator_category = std::forward_iterator_tag;
    reference operator*() const { return ptr_->data; }


    auto &operator++() {
        ptr_ = ptr_->next;
        return *this;
    }

    auto operator++(int) {
        auto it = *this;
        ++*this;
        return it;
    }

    template<bool R>
    bool operator==(const ListOfIntsIterator<R> &rhs) const {
        return ptr_ == rhs.ptr_;
    }

    template<bool R>
    bool operator!=(const ListOfIntsIterator<R> &rhs) const {
        return ptr_ != rhs.ptr_;
    }

    operator ListOfIntsIterator<true>() const {
        return ListOfIntsIterator<true>{ptr_};
    }
};

class ListOfInts {
    ListNode *head_ = nullptr;
    ListNode *tail_ = nullptr;
public:
    using const_iterator = ListOfIntsIterator<true>;
    using iterator = ListOfIntsIterator<false>;

    iterator begin() { return iterator{head_}; }

    iterator end() { return iterator{nullptr}; }

    const_iterator begin() const { return const_iterator{head_}; }

    const_iterator end() const { return const_iterator{nullptr}; }
};

template<typename Iterator, typename Predicate>
int count_if(Iterator begin,Iterator end, Predicate pred) {
    using Traits = std::iterator_traits<Iterator>;
    auto sum = typename Traits::difference_type{};
    for (auto it = begin;it != end;++it) {
        if (pred(*it)){
            ++sum;
        }
    }
    return sum;
}

template<typename Iterator>
auto distance(Iterator begin,Iterator end) {
    using Traits = std::iterator_traits<Iterator>;
    if constexpr(std::is_base_of_v<std::random_access_iterator_tag,
            typename Traits::iterator_category>){
        return (end - begin);
    }else {
        auto result = typename Traits::difference_type{};
        for (auto it = begin;it != end; ++it) {
            ++result;
        }
        return result;
    }
}

int main() {
    int i = 0;
}
