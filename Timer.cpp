#include "Timer.hpp"

Timer::Timer(uint64_t interval) : interval(interval), stopFlag(false), pauseFlag(true), timerThread(std::thread(fun)) {
    this->pauseFunction = [](void) -> bool { return false; };
    this->stopFunction = [](void) -> bool { return false; };
    this->function = [](void) -> void {};
    this->timerThread.detach();
}

Timer::Timer(const char* time) : stopFlag(false), pauseFlag(true), dateFlag(true) {
    std::tm tim {};
    strptime(time, "%d.%m.%Y %H:%M:%S", &tim);
    std::time_t targetTime = std::mktime(&tim);
    std::time_t actualTime;
    std::time(&actualTime);
    this->interval = (targetTime - actualTime) * 1000;
    this->pauseFunction = [](void) -> bool { return false; };
    this->stopFunction = [](void) -> bool { return false; };
    this->function = [](void) -> void {};
    this->timerThread = std::thread(fun);
    this->timerThread.detach();
}

Timer::Timer(const std::string& time) : Timer(time.c_str()) { }

Timer::~Timer() {
}

void Timer::start() {
    this->pauseFlag = false;
    this->condPokracuj.notify_one();
}

void Timer::stop() {
    this->stopFlag = true;
}

void Timer::pause() {
    this->pauseFlag = true;
}

void Timer::setStopFunction(bool (*fun)()) {
    this->stopFunction = fun;
}

void Timer::setPauseFunction(bool (*fun)()) {
    pauseFunction = fun;
}

void Timer::setInterval(uint32_t interval) {
    this->interval = interval;
}

void Timer::setTime(const std::string& time) {
    std::tm tim {};
    strptime(time.c_str(), "%d.%m.%Y %H:%M:%S", &tim);
    std::time_t targetTime = std::mktime(&tim);
    std::time_t actualTime;
    std::time(&actualTime);
    this->interval = (targetTime - actualTime) * 1000;
    this->dateFlag = true;
}
