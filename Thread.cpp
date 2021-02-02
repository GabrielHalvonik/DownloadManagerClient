#include "Thread.hpp"
#include "DownloadCommands.hpp"

PriorThread::PriorThread(uint32_t priority, UrlDownload* download, Thread* thread) : priority(priority), download(download), thread(thread) {}

bool operator<(const PriorThread& first, const PriorThread& second) {
    return first.priority < second.priority;
}

