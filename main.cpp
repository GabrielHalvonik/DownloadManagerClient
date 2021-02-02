#include <iostream>
#include "HttpDownload.hpp"
#include "FtpDownload.hpp"
#include "Timer.hpp"
#include <filesystem>
#include "FileSystemCommands.hpp"
#include "UtilCommands.hpp"
#include "DownloadCommands.hpp"
#include <map>

int main() {
    std::map<std::string, ICommand*> commands {
        { "ls", new ListCommand },
        { "pwd", new PathCommand },
        { "rm", new RemoveCommand },
        { "rmdir", new RemoveDirectoryCommand },
        { "mkdir", new MakeDirectoryCommand },
        { "touch", new TouchFileCommand },
        { "cd", new EnterDirectoryCommand },
        { "cat", new PrintFileCommand },
        { "cp", new CopyCommand },
        { "cpdir", new CopyDirectoryCommand },
        { "mv", new MoveCommand },
        { "time", new TimeCommand },
        { "http", new HttpDownloadCommand },
        { "https", new HttpsDownloadCommand },
        { "ftp", new FtpDownloadCommand },
        { "async_http", new AsyncHttpDownloadCommand },
        { "async_https", new AsyncHttpsDownloadCommand },
        { "async_ftp", new AsyncFtpDownloadCommand },
        { "future_http", new FutureHttpDownloadCommand },
        { "future_https", new FutureHttpsDownloadCommand },
        { "future_ftp", new FutureFtpDownloadCommand },
        { "prior_http", new PriorHttpDownloadCommand },
        { "prior_https", new PriorHttpsDownloadCommand },
        { "prior_ftp", new PriorFtpDownloadCommand },
        { "url_history", new DownloadHistoryCommand },
        { "pause", new DownloadPauseCommand },
        { "resume", new DownloadResumeCommand },
        { "stop", new DownloadStopCommand },
        { "url_list", new DownloadListCommand }
    };

    DownloadHistoryCommand* history = dynamic_cast<DownloadHistoryCommand*>(commands["url_history"]);
    dynamic_cast<IDownloadCommand*>(commands["http"])->setHistoryCommand("http", history);
    dynamic_cast<IDownloadCommand*>(commands["https"])->setHistoryCommand("https", history);
    dynamic_cast<IDownloadCommand*>(commands["ftp"])->setHistoryCommand("ftp", history);
    dynamic_cast<IDownloadCommand*>(commands["async_http"])->setHistoryCommand("http", history);
    dynamic_cast<IDownloadCommand*>(commands["async_https"])->setHistoryCommand("https", history);
    dynamic_cast<IDownloadCommand*>(commands["async_ftp"])->setHistoryCommand("ftp", history);
    dynamic_cast<IDownloadCommand*>(commands["future_http"])->setHistoryCommand("http", history);
    dynamic_cast<IDownloadCommand*>(commands["future_https"])->setHistoryCommand("https", history);
    dynamic_cast<IDownloadCommand*>(commands["future_ftp"])->setHistoryCommand("ftp", history);
    dynamic_cast<IDownloadCommand*>(commands["prior_ftp"])->setHistoryCommand("ftp", history);

    DownloadPauseCommand* pause = dynamic_cast<DownloadPauseCommand*>(commands["pause"]);
    DownloadResumeCommand* resume = dynamic_cast<DownloadResumeCommand*>(commands["resume"]);
    DownloadStopCommand* stop = dynamic_cast<DownloadStopCommand*>(commands["stop"]);
    DownloadListCommand* list = dynamic_cast<DownloadListCommand*>(commands["url_list"]);
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_http"]));
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_https"]));
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_ftp"]));
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_http"]));
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_https"]));
    pause->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_ftp"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_http"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_https"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_ftp"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_http"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_https"]));
    resume->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_ftp"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_http"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_https"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_ftp"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_http"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_https"]));
    stop->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_ftp"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_http"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_https"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["async_ftp"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_http"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_https"]));
    list->addDownloadCommands(dynamic_cast<IDownloadCommand*>(commands["future_ftp"]));

    while (true) {
        std::cout << "SemPOS \t> ";
        std::string cmd {};
        std::string arg {};

        std::getline(std::cin, cmd);

        if (cmd == "exit")
            break;

        std::istringstream iss(cmd);
        std::getline(iss, cmd, ' ');
        std::getline(iss, arg);
        try {
            if (commands.count(cmd))
                commands[cmd]->execute(arg);
        } catch (...) {
            std::cerr << "Nespravny command...\n";
        }
    }

    std::map<std::string, ICommand*>::iterator it;
    for (it = commands.begin(); it != commands.end(); ++it) {
        delete it->second;
        it->second = nullptr;
    }

    std::cout << "KONIEC" << "\n";
    return 0;
}
