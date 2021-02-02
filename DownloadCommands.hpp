#pragma once

#include <thread>
#include <deque>
#include <map>
#include <queue>
#include "Commands.hpp"
#include "UrlDownload.hpp"
#include "list"
#include "Utils.hpp"
#include "Thread.hpp"

class Timer;
class IDownloadCommand;

class DownloadHistoryCommand : public ICommand {
public:
    virtual ~DownloadHistoryCommand() = default;

    void execute(const std::string& args) override;

    void addNewRecord(const std::string&);

private:
    std::list<std::string> history {};
};

class DownloadStopCommand : public ICommand {
public:
    virtual ~DownloadStopCommand() = default;

    void execute(const std::string&) override;
    void addDownloadCommands(IDownloadCommand*);

private:
    std::deque<IDownloadCommand*> commands;
};

class DownloadPauseCommand : public ICommand {
public:
    virtual ~DownloadPauseCommand() = default;

    void execute(const std::string&) override;
    void addDownloadCommands(IDownloadCommand*);

private:
    std::deque<IDownloadCommand*> commands;
};

class DownloadResumeCommand : public ICommand {
public:
    virtual ~DownloadResumeCommand() = default;

    void execute(const std::string&) override;
    void addDownloadCommands(IDownloadCommand*);

private:
    std::deque<IDownloadCommand*> commands;
};

class DownloadListCommand : public ICommand {
public:
    virtual ~DownloadListCommand() = default;

    void execute(const std::string&) override;
    void addDownloadCommands(IDownloadCommand*);

private:
    std::deque<IDownloadCommand*> commands;
};

class IDownloadCommand : public ICommand {
public:
    virtual ~IDownloadCommand() = default;

    virtual void execute(const std::string& args) override = 0;

    void parametrizeCommonDownload(const std::string& args);
    void setHistoryCommand(const std::string&, DownloadHistoryCommand*);
    void writeBeginDownloadTime(const std::string& = {}, const std::string& = {});
    void writeEndDownloadTime(const std::string& = {}, const std::string& = {});

public:
    std::list<std::string>::iterator iterator {};
    std::map<std::string, UrlDownload*> inProgress {};
protected:
    std::string url {};
    std::string filename {};
    std::map<std::string, DownloadHistoryCommand*> histories {};
};

class HttpDownloadCommand : public IDownloadCommand {
public:
    virtual ~HttpDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class HttpsDownloadCommand : public IDownloadCommand {
public:
    virtual ~HttpsDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class FtpDownloadCommand : public IDownloadCommand {
public:
    virtual ~FtpDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class AsyncDownload {
public:
    virtual ~AsyncDownload();

    void startAsync(const std::function<void(void)>&);
public:
    std::list<Thread*> threadPool {};
};

class AsyncHttpDownloadCommand : public HttpDownloadCommand, public AsyncDownload {
public:
    virtual ~AsyncHttpDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class AsyncHttpsDownloadCommand : public HttpsDownloadCommand, public AsyncDownload {
public:
    virtual ~AsyncHttpsDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class AsyncFtpDownloadCommand : public FtpDownloadCommand, public AsyncDownload {

public:
    virtual ~AsyncFtpDownloadCommand() = default;

    virtual void execute(const std::string& args) override;
};

class FutureDownload {
public:
    virtual ~FutureDownload();

    void startFuture(const std::string&, const std::function<void(void)>&);
    void parametrizeTime(IDownloadCommand*, const std::string&);
public:
    std::deque<Timer*> timerPool {};
    std::string time {};
};

class FutureHttpDownloadCommand : public HttpDownloadCommand, public FutureDownload {
public:
    virtual ~FutureHttpDownloadCommand() = default;

    void execute(const std::string& args) override;
};

class FutureHttpsDownloadCommand : public HttpsDownloadCommand, public FutureDownload {
public:
    virtual ~FutureHttpsDownloadCommand() = default;

    void execute(const std::string& args) override;

};

class FutureFtpDownloadCommand : public FtpDownloadCommand, public FutureDownload {
public:
    virtual ~FutureFtpDownloadCommand() = default;

    void execute(const std::string& args) override;
};

class PriorDownload {
public:
    virtual ~PriorDownload();

    void startPriorAsync(uint16_t, UrlDownload*, const std::function<void(void)>&);

    void reorganize();

    uint32_t parametrizePriority(std::list<std::string>&);

public:
    std::priority_queue<PriorThread> priorThreadPool {};
};


class PriorHttpDownloadCommand : public HttpDownloadCommand, public PriorDownload {
public:
    virtual ~PriorHttpDownloadCommand() = default;

    virtual void execute(const std::string&) override;
};

class PriorHttpsDownloadCommand : public HttpsDownloadCommand, public PriorDownload {
public:
    virtual ~PriorHttpsDownloadCommand() = default;

    virtual void execute(const std::string&) override;
};

class PriorFtpDownloadCommand : public FtpDownloadCommand, public PriorDownload {
public:
    virtual ~PriorFtpDownloadCommand() = default;

    virtual void execute(const std::string&) override;
};
