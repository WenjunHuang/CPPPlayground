#include <iterator>
#include <type_traits>
template <class Iterator>
auto distance(Iterator first, Iterator last, std::input_iterator_tag) {
    typename std::iterator_traits<Iterator>::difference_type result{0};
    while (first != last)
        ++first, ++result;
    return result;
}

template <class RAIter>
auto distance(RAIter first, RAIter last, std::random_access_iterator_tag) {
    return last - first;
}

template <class Iterator> auto distance(Iterator first, Iterator last) {
    return distance(
        first, last,
        typename std::iterator_traits<Iterator>::iterator_category{});
}

template <class T, std::size_t N>
std::enable_if_t<not std::is_trivially_copyable_v<T>>
copy_array(const T (&source)[N], T (&dest)[N]) {
    std::copy(source, std::end(source), dest);
}

template <class T, std::size_t N>
std::enable_if_t<std::is_trivially_copyable_v<T>>
copy_array(T const (&source)[N], T (&dest)[N]) {
    std::memcpy(dest, source, N * sizeof(T));
}

int main() {}