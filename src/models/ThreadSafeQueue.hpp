#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue {
    private:
        std::queue<T> q;
        mutable std::mutex m;
        std::condition_variable cv;
    public:
        ThreadSafeQueue() {}
        void push(T val) {
            std::lock_guard<std::mutex> lock(m);
            q.push(val);
            cv.notify_one();
        }
        T pop() {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this] { return !q.empty(); });
            T val = q.front();
            q.pop();
            return val;
        }
        bool empty() const {
            std::lock_guard<std::mutex> lock(m);
            return q.empty();
        }
};

#endif