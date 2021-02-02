#pragma once

#include <thread>
#include <chrono>

class Timer {
public:
    Timer(uint64_t = 1000);
    Timer(const char*);
    Timer(const std::string&);

    ~Timer();

    void start();
    void stop();
    void pause();

    void detach() { timerThread.detach(); }

    void setStopFunction(bool(*fun)(void));
    void setPauseFunction(bool(*fun)(void));
    void setInterval(uint32_t interval);
    void setTime(const std::string&);

    void setRunningProgram(std::function<void(void)> function) { this->function = function; }

private:
    std::function<void(void)> fun = [this](void) -> void {
        while (true) {
            if (this->pauseFlag == true) {
                std::unique_lock<std::mutex> lock(this->mutex);
                condPokracuj.wait(lock);
            } if (this->stopFlag == true) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(this->interval));
            this->function();

            if (pauseFunction())
                this->pauseFlag = true;
            if (stopFunction() || this->dateFlag) {
                this->stopFlag = true;
                break;
            }
        }
    };

private:
    uint64_t interval {};
    bool stopFlag {};
    bool pauseFlag {};
    bool dateFlag {};
    std::thread timerThread {};
    std::mutex mutex {};
    std::condition_variable condPokracuj {};
    bool(*pauseFunction)(void) {};
    bool(*stopFunction)(void) {};
    std::function<void(void)> function;
};

