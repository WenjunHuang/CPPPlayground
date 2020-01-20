//
// Created by HUANG WEN JUN on 2020/1/7.
//
#pragma once
#include <thread>

class ScopedThread {
    std::thread _t;
  public:
    explicit ScopedThread(std::thread t):_t{std::move(t)} {
        if (!_t.joinable())
            throw std::logic_error("No thread");
    }
    ~ScopedThread(){
        _t.join();
    }

    ScopedThread(ScopedThread const&) = delete;
    ScopedThread& operator=(ScopedThread const&) = delete;
};


class JoiningThread{
    std::thread _t;
  public:
    JoiningThread() noexcept  = default;
    template<typename Callable,typename ... Args>
    explicit JoiningThread(Callable&& func,Args&& ... args):_t(std::forward<Callable>(func),std::forward<Args>(args)...) { }
    explicit JoiningThread(std::thread t) noexcept:_t(std::move(t)){}
    JoiningThread(JoiningThread&& other) noexcept :_t(std::move(other.t)){}

    JoiningThread& operator=(JoiningThread&& other) {
        if (joinable())
            join();
        _t = std::move(other._t);
        return *this;
    }

    JoiningThread& operator=(std::thread other) noexcept  {
        if (joinable())
            join();
        _t = std::move(other);
        return *this;
    }

    ~JoiningThread() {
        if (joinable())
            join();
    }

    void swap(JoiningThread& other) noexcept  {
        _t.swap(other._t);
    }

    std::thread::id getId() const noexcept {
        return _t.get_id();
    }

    bool joinable() const noexcept {
        return _t.joinable();
    }


    void join() {
        _t.join();
    }

    void detach() {
        _t.detach();
    }

    std::thread& asThread() noexcept {
        return _t;
    }

    std::thread& asThread() const noexcept {
        return _t;
    }
};