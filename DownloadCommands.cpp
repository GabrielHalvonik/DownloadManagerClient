#include "DownloadCommands.hpp"
#include "HttpDownload.hpp"
#include "FtpDownload.hpp"
#include "Timer.hpp"
#include <iostream>

void DownloadHistoryCommand::execute(const std::string&) {
    for (const std::string& temp : history) {
        std::cout << "\t> " << temp << "\n";
    }
}

void DownloadHistoryCommand::addNewRecord(const std::string& record) {
    this->history.push_back(record);
}

void DownloadStopCommand::execute(const std::string& args) {
    if (!args.empty()) {
        for (IDownloadCommand* temp : this->commands) {
            if (temp->inProgress.count(args))
                temp->inProgress[args]->stop();
        }
    } else {
        for (IDownloadCommand* temp : this->commands) {
            for (auto& [key, ptr] : temp->inProgress) {
                ptr->stop();
            }
        }
    }
}

void DownloadStopCommand::addDownloadCommands(IDownloadCommand* command) {
    this->commands.push_back(command);
}

void DownloadPauseCommand::execute(const std::string& args) {
    if (!args.empty()) {
        for (IDownloadCommand* temp : this->commands) {
            if (temp->inProgress.count(args))
                temp->inProgress[args]->pause();
        }
    } else {
        for (IDownloadCommand* temp : this->commands) {
            for (auto& [key, ptr] : temp->inProgress) {
                ptr->pause();
            }
        }
    }
}

void DownloadPauseCommand::addDownloadCommands(IDownloadCommand* command) {
    this->commands.push_back(command);
}

void DownloadResumeCommand::execute(const std::string& args) {
    if (!args.empty()) {
        for (IDownloadCommand* temp : this->commands) {
            if (temp->inProgress.count(args))
                temp->inProgress[args]->resume();
        }
    } else {
        for (IDownloadCommand* temp : this->commands) {
            for (auto& [key, ptr] : temp->inProgress) {
                ptr->resume();
            }
        }
    }
}

void DownloadResumeCommand::addDownloadCommands(IDownloadCommand* command) {
    this->commands.push_back(command);
}

void DownloadListCommand::execute(const std::string&) {
    for (IDownloadCommand* temp : this->commands) {
        for (auto& [key, ptr] : temp->inProgress) {
            std::fstream fs(ptr->getFilename(), std::ios_base::ate | std::ios_base::app);
            uint64_t size {};
            size = fs.tellg() / 1000;
            std::cout << "\t> (" << key << ")\t" << ptr->getUrl() << " ... " << ptr->getFilename() << "(" << size << "kB)" << std::endl;
        }
    }
}

void DownloadListCommand::addDownloadCommands(IDownloadCommand* command) {
    this->commands.push_back(command);
}

void IDownloadCommand::setHistoryCommand(const std::string& name, DownloadHistoryCommand* history) {
    if (this->histories.count(name) == 0)
        this->histories.insert( {name, history} );
}

void IDownloadCommand::writeBeginDownloadTime(const std::string& protocol, const std::string& filename) {
    std::stringstream ss {};
    ss << "-> (" << Utils::getCurrentTime() << ")\t(from) " << this->url << "\t(to) " << filename;
    if (this->histories.count(protocol))
        this->histories[protocol]->addNewRecord(ss.str());
}

void IDownloadCommand::writeEndDownloadTime(const std::string& protocol, const std::string& filename) {
    std::stringstream ss {};
    ss << "<- (" << Utils::getCurrentTime() << ")\t(from) " << this->url << "\t(to) " << filename;
    if (this->histories.count(protocol))
        this->histories[protocol]->addNewRecord(ss.str());
}

void IDownloadCommand::parametrizeCommonDownload(const std::string& arg) {
    ICommand::parametrizeCommand(arg);
    if (ICommand::args.size() < 2) {
        std::cerr << "\t> Nespravny format... [http/ftp] [url] [filename]\n";
    } else {
        iterator = ICommand::args.begin();
        this->url = *iterator++;
        this->filename = *iterator++;
    }
}

void HttpDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    HttpDownload* httpDownload = new HttpDownload;
    std::stringstream pointerStr {};
    pointerStr << reinterpret_cast<void*>(httpDownload);
    httpDownload->setUrl(IDownloadCommand::url.c_str());
    httpDownload->setNewFilename(IDownloadCommand::filename.c_str());
    IDownloadCommand::writeBeginDownloadTime("http", IDownloadCommand::filename.c_str());
    IDownloadCommand::inProgress.insert({ pointerStr.str(), httpDownload });
    httpDownload->download();
    IDownloadCommand::inProgress.erase(pointerStr.str());
    IDownloadCommand::writeEndDownloadTime("http", IDownloadCommand::filename.c_str());
    delete httpDownload;
    httpDownload = nullptr;
}

void HttpsDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    HttpsDownload* httpsDownload = new HttpsDownload;
    std::stringstream pointerStr {};
    pointerStr << reinterpret_cast<void*>(httpsDownload);
    httpsDownload->setUrl(IDownloadCommand::url.c_str());
    httpsDownload->setNewFilename(IDownloadCommand::filename.c_str());
    IDownloadCommand::writeBeginDownloadTime("https", IDownloadCommand::filename.c_str());
    IDownloadCommand::inProgress.insert({ pointerStr.str(), httpsDownload });
    httpsDownload->download();
    IDownloadCommand::inProgress.erase(pointerStr.str());
    IDownloadCommand::writeEndDownloadTime("https", IDownloadCommand::filename.c_str());
    delete httpsDownload;
    httpsDownload = nullptr;
}

void FtpDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    FtpDownload* ftpDownload = new FtpDownload;
    std::stringstream pointerStr {};
    pointerStr << reinterpret_cast<void*>(ftpDownload);
    ftpDownload->setUrl(IDownloadCommand::url.c_str());
    ftpDownload->setNewFilename(IDownloadCommand::filename.c_str());
    IDownloadCommand::writeBeginDownloadTime("ftp", IDownloadCommand::filename.c_str());
    IDownloadCommand::inProgress.insert({ pointerStr.str(), ftpDownload });
    ftpDownload->download();
    IDownloadCommand::inProgress.erase(pointerStr.str());
    IDownloadCommand::writeEndDownloadTime("ftp", IDownloadCommand::filename.c_str());
    delete ftpDownload;
    ftpDownload = nullptr;
}

AsyncDownload::~AsyncDownload() {
    std::list<Thread*>::iterator iterator;
    for (iterator = this->threadPool.begin(); iterator != this->threadPool.end(); ++iterator) {
        (*iterator)->pause();
        delete *iterator;
        *iterator = nullptr;
    }
}

void AsyncDownload::startAsync(const std::function<void(void)>& function) {
    Thread* async = new Thread;
    this->threadPool.push_back(async);
    async->setRunningProgram(function);
    async->start();
}

FutureDownload::~FutureDownload() {
    std::deque<Timer*>::iterator iterator;
    (*iterator)->stop();
    for (iterator = this->timerPool.begin(); iterator != this->timerPool.end(); ++iterator) {
        delete *iterator;
        *iterator = nullptr;
    }
}

void FutureDownload::startFuture(const std::string& time, const std::function<void(void)>& function) {
    Timer* future = new Timer(time);
    this->timerPool.push_back(future);
    future->setRunningProgram(function);
    future->start();

}

void FutureDownload::parametrizeTime(IDownloadCommand* command, const std::string& args) {
    command->parametrizeCommonDownload(args);
    std::stringstream ss {};
    ss << *command->iterator++ << " " << *command->iterator++;
    this->time = ss.str();
}

void AsyncHttpDownloadCommand::execute(const std::string& args) {
    AsyncDownload::startAsync(std::bind([this](AsyncHttpDownloadCommand*, const std::string& args) -> void {
        HttpDownloadCommand::execute(args);
    }, this, args));
}

void AsyncHttpsDownloadCommand::execute(const std::string& args) {
    AsyncDownload::startAsync(std::bind([this](AsyncHttpsDownloadCommand*, const std::string& args) -> void {
        HttpsDownloadCommand::execute(args);
    }, this, args));
}

void AsyncFtpDownloadCommand::execute(const std::string& args) {
    AsyncDownload::startAsync(std::bind([this](AsyncFtpDownloadCommand*, const std::string& args) -> void {
        FtpDownloadCommand::execute(args);
    }, this, args));
}

void FutureHttpDownloadCommand::execute(const std::string& args) {
    FutureDownload::parametrizeTime(this, args);
    FutureDownload::startFuture(FutureDownload::time, std::bind([this](FutureHttpDownloadCommand* , const std::string& args) -> void {
                                    HttpDownloadCommand::execute(args);
                                }, this, args));
}

void FutureHttpsDownloadCommand::execute(const std::string& args) {
    FutureDownload::parametrizeTime(this, args);
    FutureDownload::startFuture(FutureDownload::time, std::bind([this](FutureHttpsDownloadCommand* , const std::string& args) -> void {
                                    HttpsDownloadCommand::execute(args);
                                }, this, args));
}

void FutureFtpDownloadCommand::execute(const std::string& args) {
    FutureDownload::parametrizeTime(this, args);
    FutureDownload::startFuture(FutureDownload::time, std::bind([this](FutureFtpDownloadCommand* , const std::string& args) -> void {
                                    FtpDownloadCommand::execute(args);
                                }, this, args));
}


PriorDownload::~PriorDownload() {
    while (!this->priorThreadPool.empty()) {
        delete this->priorThreadPool.top().thread;
        this->priorThreadPool.pop();
    }
}

void PriorDownload::startPriorAsync(uint16_t priority, UrlDownload* download, const std::function<void(void)>& function) {
    Thread* priorThread = new Thread;
    priorThread->setRunningProgram(function);
    if (priorThreadPool.empty() || priorThreadPool.top().priority > priority) {
        if (!priorThreadPool.empty())
            priorThreadPool.top().download->pause();
        priorThread->start();
        priorThread->setAfterProgram(std::bind([](PriorDownload* priorDownload) -> void {
            priorDownload->reorganize();
        }, this));
    }
    this->priorThreadPool.push( {priority, download, priorThread} );
}

void PriorDownload::reorganize() {
    std::cout << "Reorganizing..." << "\n";
    if (this->priorThreadPool.top().thread->getStopFlag()) {
        delete this->priorThreadPool.top().thread;
        this->priorThreadPool.pop();
    }

    if (!this->priorThreadPool.empty()) {
        std::cout << "starting..." << this->priorThreadPool.top().priority << "\n";
        this->priorThreadPool.top().thread->start();
    }
}

uint32_t PriorDownload::parametrizePriority(std::list<std::string>& args) {
    uint32_t res {};
    try {
        res = std::stoi(*(--args.end()));
    }  catch (std::exception& ex) {
        std::cerr << ex.what();
    }
    return res;
}

void PriorHttpDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    uint32_t prior = PriorDownload::parametrizePriority(ICommand::args);
    HttpDownload* httpDownload = new HttpDownload;
    httpDownload->setUrl(IDownloadCommand::url.c_str());
    httpDownload->setNewFilename(IDownloadCommand::filename.c_str());
    PriorDownload::startPriorAsync(prior, httpDownload, std::bind([](HttpDownload* httpDownload) -> void {
                                       httpDownload->download();
                                   }, httpDownload));
}

void PriorHttpsDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    uint32_t prior = PriorDownload::parametrizePriority(ICommand::args);
    HttpsDownload* httpsDownload = new HttpsDownload;
    httpsDownload->setUrl(IDownloadCommand::url.c_str());
    httpsDownload->setNewFilename(IDownloadCommand::filename.c_str());
    PriorDownload::startPriorAsync(prior, httpsDownload, std::bind([](HttpsDownload* httpsDownload) -> void {
                                       httpsDownload->download();
                                   }, httpsDownload));
}

void PriorFtpDownloadCommand::execute(const std::string& args) {
    IDownloadCommand::parametrizeCommonDownload(args);
    uint32_t prior = PriorDownload::parametrizePriority(ICommand::args);
    FtpDownload* ftpDownload = new FtpDownload;
    ftpDownload->setUrl(IDownloadCommand::url.c_str());
    ftpDownload->setNewFilename(IDownloadCommand::filename.c_str());
    PriorDownload::startPriorAsync(prior, ftpDownload, std::bind([this](PriorFtpDownloadCommand*, FtpDownload* httpDownload) -> void {
                                       IDownloadCommand::writeBeginDownloadTime("ftp", IDownloadCommand::filename.c_str());
                                       httpDownload->download();
                                       IDownloadCommand::writeEndDownloadTime("ftp", IDownloadCommand::filename.c_str());
                                   }, this, ftpDownload));
}
