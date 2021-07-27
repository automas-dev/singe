#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "log.hpp"

using namespace std::chrono_literals;

class DispatchQueue {
public:
    enum QueueType { Serial, Concurrent };

private:
    using Fn = std::function<void()>;

    std::vector<std::thread> workers;
    std::mutex m;
    std::condition_variable cv;
    std::atomic<bool> running;
    std::queue<Fn> taskQueue;

    void worker(size_t id) {
        SPDLOG_DEBUG("Worker {} starting", id);
        while (running) {
            std::unique_lock lk(m);
            cv.wait(lk, [&]() {
                return !running || !taskQueue.empty();
            });

            if (!running)
                break;

            auto task = taskQueue.front();
            taskQueue.pop();
            lk.unlock();
            task();
        }
        SPDLOG_DEBUG("Worker {} ending", id);
    }

public:
    DispatchQueue(QueueType type) {
        workers.emplace_back(&DispatchQueue::worker, this, 0);
        if (type == Concurrent) {
            size_t n = std::thread::hardware_concurrency();
            for (size_t i = 1; i < n; i++) {
                workers.emplace_back(&DispatchQueue::worker, this, i);
            }
        }
    }

    bool working() {
        std::unique_lock lk(m);
        return !taskQueue.empty();
    }

    void stop() {
        running = false;
        cv.notify_all();
        for (auto & worker : workers) {
            worker.join();
        }
    }

    void join() {
        while (working()) std::this_thread::yield();
        stop();
    }

    void add(std::function<void()> task) {
        std::unique_lock lk(m);
        taskQueue.push(task);
        lk.unlock();
        cv.notify_one();
    }
};