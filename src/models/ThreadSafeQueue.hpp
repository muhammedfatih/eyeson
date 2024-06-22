#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable cv;

public:
    ThreadSafeQueue() {}

    void push(T val) {
        std::unique_lock<std::mutex> lock(m);
        q.push(val);
        cv.notify_one();
    }

    bool tryPop(T& val) {
        std::unique_lock<std::mutex> lock(m);
        if (q.empty()) {
            return false;
        }
        val = q.front();
        q.pop();
        return true;
    }

    bool empty() const {
        std::unique_lock<std::mutex> lock(m);
        return q.empty();
    }
};

#endif