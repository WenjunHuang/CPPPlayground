//
// Created by rick on 2021/5/6.
//
#include <queue>
#include <rx.hpp>
using namespace rxcpp;
using namespace rxcpp::schedulers;

template <typename TimePoint>
struct TimeSchedulable {
    TimePoint when;
    schedulable what;
    using time_point_type = TimePoint;
};

// sorts TimeSchedulable items in priority order sorted on value
// of TimeSchedulable.when. Items with equal values for when are sorted in
// fifo order.
template <typename TimePoint>
class SchedulableQueue {
public:
    using item_type = TimeSchedulable<TimePoint>;
    using elem_type = std::pair<item_type, int64_t>;
    using container_type = std::vector<elem_type>;
    using const_reference = const item_type&;

private:
    struct compare_elem {
        bool operator()(const elem_type& lhs, const elem_type& rhs) const
        {
            if (lhs.first.when == rhs.first.when) {
                return lhs.second > rhs.second;
            } else {
                return lhs.first.when > rhs.first.when;
            }
        }
    };

    using queue_type = std::priority_queue<
        elem_type,
        container_type,
        compare_elem>;

    queue_type q;
    int64_t ordinal;

public:
    SchedulableQueue()
        : ordinal { 0 }
    {
    }

    const_reference top() const
    {
        return q.top().first;
    }

    void pop()
    {
        q.pop();
    }

    bool empty() const
    {
        return q.empty();
    }

    void push(const item_type& value)
    {
        q.push(elem_type { value, ordinal++ });
    }
    void push(item_type&& value)
    {
        q.push(elem_type { std::move(value), ordinal++ });
    }
};

struct ActionQueue {
    using this_type = ActionQueue;
    using clock = scheduler_base::clock_type;
    using item_type = TimeSchedulable<clock::time_point>;

private:
    using queue_item_time = SchedulableQueue<item_type::time_point_type>;

public:
    struct current_thread_queue_type {
        std::shared_ptr<worker_interface> w;
        recursion r;
        queue_item_time q;
    };

private:
    static current_thread_queue_type*& current_thread_queue()
    {
        static thread_local current_thread_queue_type* q;
        return q;
    }

public:
    static bool owned()
    {
        return current_thread_queue() != nullptr;
    }

    static const std::shared_ptr<worker_interface>& get_worker_interface()
    {
        return current_thread_queue()->w;
    }

    static recursion& get_recursion()
    {
        return current_thread_queue()->r;
    }
    static bool empty()
    {
        if (current_thread_queue() == nullptr) {
            std::terminate();
        }
        return current_thread_queue()->q.empty();
    }
    static queue_item_time::const_reference top()
    {
        if (current_thread_queue() == nullptr) {
            std::terminate();
        }
        return current_thread_queue()->q.top();
    }
    static void pop()
    {
        if (current_thread_queue() == nullptr) {
            std::terminate();
        }
        auto& state = current_thread_queue();
        state->q.pop();
        if (state->q.empty()) {
            // allow recursion
            state->r.reset(true);
        }
    }
    static void push(item_type item)
    {
        auto& state = current_thread_queue();
        if (state == nullptr) {
            std::terminate();
        }

        if (!item.what.is_subscribed())
            return;

        state->q.push(std::move(item));
        // disallow recursion
        state->r.reset(false);
    }
    static std::shared_ptr<worker_interface> ensure(std::shared_ptr<worker_interface> w)
    {
        if (current_thread_queue() != nullptr) {
            std::terminate();
        }

        // create and publish new queue
        current_thread_queue() = new current_thread_queue_type();
        current_thread_queue()->w = w;
        return w;
    }
    static std::unique_ptr<current_thread_queue_type> create(std::shared_ptr<worker_interface> w)
    {
        std::unique_ptr<current_thread_queue_type> result = std::make_unique<current_thread_queue_type>();
        result->w = std::move(w);
        return result;
    }
    static void set(current_thread_queue_type* q)
    {
        if (current_thread_queue() != nullptr)
            std::terminate();

        // publish new queue
        current_thread_queue() = q;
    }
    static void destroy(current_thread_queue_type* q)
    {
        delete q;
    }
    static void destroy()
    {
        if (current_thread_queue() == nullptr)
            std::terminate();
        destroy(current_thread_queue());
        current_thread_queue() = nullptr;
    }
};

struct MyNewThread : scheduler_interface {
private:
    using this_type = MyNewThread;
    MyNewThread(const this_type&);

    struct new_worker : worker_interface {
    private:
        using this_type = new_worker;
        using queue_type = ActionQueue;
        new_worker(const this_type&);

        struct new_worker_state : std::enable_shared_from_this<new_worker_state> {
            using queue_item_time = SchedulableQueue<typename clock_type::time_point>;
            using item_type = typename queue_item_time::item_type;

            virtual ~new_worker_state() { }

            explicit new_worker_state(composite_subscription cs)
                : lifetime { cs }
            {
            }

            composite_subscription lifetime;
            mutable std::mutex lock;
            mutable std::condition_variable wake;
            mutable queue_item_time q;
            std::thread worker;
            recursion r;
        };

        std::shared_ptr<new_worker_state> state;

    public:
        virtual ~new_worker() { }

        explicit new_worker(std::shared_ptr<new_worker_state> ws)
            : state { ws }
        {
        }
    };
};
int main()
{
}
