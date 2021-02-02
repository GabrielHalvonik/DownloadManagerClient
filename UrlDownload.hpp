#pragma once

#include <curl/curl.h>
#include <fstream>
#include <sstream>

class UrlDownload {
public:
    UrlDownload();
    virtual ~UrlDownload();

    UrlDownload& setNewFilename(const char*);
    virtual void download() = 0;

    void pause();
    void resume();
    void stop();

    std::string getUrl() const;
    std::string getFilename() const;

protected:
    size_t(*writeFunction)(void*, size_t, size_t, void*);

protected:
    CURL* curl {};
    std::string filename {};
    std::string url {};
    std::stringstream urlBuilder {};
};
