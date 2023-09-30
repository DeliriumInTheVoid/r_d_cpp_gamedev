#pragma once

#include <deque>
#include <mutex>


template<typename T>
class ts_deque final
{
public:
    ts_deque() = default;
    ts_deque(const ts_deque<T>&) = delete;
    ~ts_deque() = default;

public:
    const T& front()
    {
        std::scoped_lock lock(mux_queue_);
        return deq_queue_.front();
    }

    const T& back()
    {
        std::scoped_lock lock(mux_queue_);
        return deq_queue_.back();
    }

    T pop_front()
    {
        std::scoped_lock lock(mux_queue_);
        auto t = std::move(deq_queue_.front());
        deq_queue_.pop_front();
        return t;
    }

    // Removes and returns item from back of Queue
    T pop_back()
    {
        std::scoped_lock lock(mux_queue_);
        auto t = std::move(deq_queue_.back());
        deq_queue_.pop_back();
        return t;
    }

    void push_back(const T& item)
    {
        std::scoped_lock lock(mux_queue_);
        deq_queue_.emplace_back(std::move(item));

        std::unique_lock ul(mux_blocking_);
        cv_blocking_.notify_one();
    }

    void push_back(T&& item)
    {
        std::scoped_lock lock(mux_queue_);
        deq_queue_.emplace_back(std::move(item));

        std::unique_lock ul(mux_blocking_);
        cv_blocking_.notify_one();
    }

    void push_front(const T& item)
    {
        std::scoped_lock lock(mux_queue_);
        deq_queue_.emplace_front(std::move(item));

        std::unique_lock ul(mux_blocking_);
        cv_blocking_.notify_one();
    }

    bool empty()
    {
        std::scoped_lock lock(mux_queue_);
        return deq_queue_.empty();
    }

    size_t count()
    {
        std::scoped_lock lock(mux_queue_);
        return deq_queue_.size();
    }

    void clear()
    {
        std::scoped_lock lock(mux_queue_);
        deq_queue_.clear();
    }

    void wait()
    {
        while (empty())
        {
            std::unique_lock ul(mux_blocking_);
            cv_blocking_.wait(ul);
        }
    }

private:
    std::mutex mux_queue_;
    std::deque<T> deq_queue_;
    std::condition_variable cv_blocking_;
    std::mutex mux_blocking_;
};
