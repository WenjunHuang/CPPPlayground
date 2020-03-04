//
// Created by HUANG WEN JUN on 2020/3/3.
//
#include <cmath>
#include <experimental/coroutine>
#include <iostream>
#include <type_traits>

template <typename T> class generator;

template <typename T> class generator_promise {
  public:
    using value_type     = std::remove_reference_t<T>;
    using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
    using pointer_type   = value_type*;

    generator_promise() = default;

    generator<T> get_return_object() noexcept {
        using coroutine_handle =
            std::experimental::coroutine_handle<generator_promise<T>>;
        return generator<T>{coroutine_handle::from_promise(*this)};
    }

    constexpr std::experimental::suspend_always initial_suspend() const {
        return {};
    }
    constexpr std::experimental::suspend_always final_suspend() const {
        return {};
    }

    template <typename U                                                 = T,
              std::enable_if_t<!std::is_rvalue_reference<U>::value, int> = 0>
    std::experimental::suspend_always
    yield_value(std::remove_reference_t<T>& value) noexcept {
        m_value = std::addressof(value);
        return {};
    }

    std::experimental::suspend_always
    yield_value(std::remove_reference_t<T>&& value) noexcept {
        m_value = std::addressof(value);
        return {};
    }

    void unhandled_exception() { m_exception = std::current_exception(); }

    void return_void() {}

    reference_type value() const noexcept {
        return static_cast<reference_type>(*m_value);
    }

    // Don't allow any use of 'co_await' inside the generator coroutine.
    template <typename U>
    std::experimental::suspend_never await_transform(U&& value) = delete;

    void rethrow_if_exception() {
        if (m_exception) {
            std::rethrow_exception(m_exception);
        }
    }

  private:
    pointer_type m_value;
    std::exception_ptr m_exception;
};

struct generator_sentinel {};

template <typename T> class generator_iterator {
    using coroutine_handle =
        std::experimental::coroutine_handle<generator_promise<T>>;

  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = typename generator_promise<T>::value_type;
    using reference         = typename generator_promise<T>::reference_type;
    using pointer           = typename generator_promise<T>::pointer_type;

    generator_iterator() noexcept : m_coroutine{nullptr} {}

    explicit generator_iterator(coroutine_handle coroutine) noexcept
        : m_coroutine{coroutine} {}

    friend bool operator==(const generator_iterator& it,
                           generator_sentinel) noexcept {
        return !it.m_coroutine || it.m_coroutine.done();
    }

    friend bool operator!=(const generator_iterator& it,
                           generator_sentinel s) noexcept {
        return !(it == s);
    }

    friend bool operator==(generator_sentinel s,
                           const generator_iterator& it) noexcept {
        return (it == s);
    }

    friend bool operator!=(generator_sentinel s,
                           const generator_iterator& it) noexcept {
        return !(it == s);
    }

    generator_iterator& operator++() {
        m_coroutine.resume();
        if (!m_coroutine.done()) {
            m_coroutine.promise().rethrow_if_exception();
        }

        return *this;
    }

    void operator++(int) { (void)operator++(); }

    reference operator*() const noexcept {
        return m_coroutine.promise().value();
    }

    pointer operator->() const noexcept { return std::addressof(operator*()); }

  private:
    coroutine_handle m_coroutine;
};

template <typename T> class [[nodiscard]] generator {
  public:
    using promise_type = generator_promise<T>;
    using iterator     = generator_iterator<T>;

    generator() noexcept : m_coroutine{nullptr} {}

    generator(std::experimental::coroutine_handle<promise_type> && handle)
        : m_coroutine{std::move(handle)} {}

    generator(generator && other) noexcept : m_coroutine{other.m_coroutine} {
        other.m_coroutine = nullptr;
    }

    generator(const generator& other) = delete;

    ~generator() {
        if (m_coroutine)
            m_coroutine.destroy();
    }

    void swap(generator & other) noexcept {
        std::swap(m_coroutine, other.m_coroutine);
    }

    generator& operator=(generator other) noexcept {
        swap(other);
        return *this;
    }

    iterator begin() {
        if (m_coroutine) {
            m_coroutine.resume();
            if (m_coroutine.done()) {
                m_coroutine.promise().rethrow_if_exception();
            }
        }
        return iterator{m_coroutine};
    }

    generator_sentinel end() noexcept { return generator_sentinel{}; }

  private:
    friend class generator_promise<T>;
    std::experimental::coroutine_handle<promise_type> m_coroutine;
};

template <typename T> void swap(generator<T>& a, generator<T>& b) { a.swap(b); }

generator<const std::uint64_t> fibonacci() {
    std::uint64_t a = 0, b = 1;
    while (true) {
        co_yield b;
        auto tmp = a;
        a        = b;
        b += tmp;
    }
}

generator<const std::uint64_t> prime_numbers() {
    std::uint64_t b = 1;
    while (true) {
        if (b == 1 || b == 2) {
            co_yield b;
            b += 1;
        }
        else {
            auto a       = std::floor(std::sqrt(b));
            bool isPrime = true;
            for (auto i = 2; i <= a; i++) {
                if (b % i == 0) {
                    isPrime = false;
                }
            }

            if (isPrime)
                co_yield b;

            b += 1;
        }
    }
}

int main() {
    for (auto i : fibonacci()) {
        if (i > 1'000'000)
            break;
        std::cout << i << std::endl;
    }

    std::cout << "next to find prime numbers\n";
    for (auto p : prime_numbers()) {
        if (p > 10000)
            break;
        std::cout << p << std::endl;
    }
}