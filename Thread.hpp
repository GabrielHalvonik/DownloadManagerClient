#pragma once

#include <thread>
#include <sstream>
#include <type_traits>

class UrlDownload;

class Thread {
public:
    Thread() : pauseFlag(true), thread(std::thread(function)) {
        this->thread.detach();
    }

    Thread(const std::function<void(void)>& running) : pauseFlag(true), runningFunction(running), thread(std::thread(function)) {
        this->thread.detach();
    }

    ~Thread() {
        if (!this->stopFlag || !this->pauseFlag)
            this->stopFunction();
    }

    void setRunningProgram(const std::function<void(void)>& running) {
        this->runningFunction = running;
    }

    void setAfterProgram(const std::function<void(void)>& after) {
        this->afterFunction = after;
    }

    inline bool getStopFlag() const { return this->stopFlag; }

    inline void start() { this->pauseFlag = false; this->condPokracuj.notify_one(); }

    inline void pause() { this->pauseFlag = true; }

private:
    void stopFunction() {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condUkonci.wait(lock, [this](void) -> bool {
            return stopFlag == false;
        });
    }

private:
    std::function<void(void)> function = [this](void) -> void {
        if (this->pauseFlag) {
            std::unique_lock<std::mutex> lock(this->mutex);
            this->condPokracuj.wait(lock);
        }
        this->runningFunction();
        this->stopFlag = true;
        this->condUkonci.notify_one();
    };

private:
    bool stopFlag {};
    bool pauseFlag {};
    std::mutex mutex {};
    std::condition_variable condUkonci {};
    std::condition_variable condPokracuj {};
    std::function<void(void)> runningFunction {};
    std::function<void(void)> afterFunction {};
    std::thread thread {};
};

struct PriorThread {
    PriorThread(uint32_t priority, UrlDownload* download, Thread* thread);

    uint32_t priority {};
    UrlDownload* download {};
    Thread* thread {};

    friend bool operator<(const PriorThread&, const PriorThread&);
};


