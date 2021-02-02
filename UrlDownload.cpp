#include "UrlDownload.hpp"
#include <iostream>

UrlDownload::UrlDownload()  {
    this->writeFunction = [](void* buffer, size_t pocet, size_t velkost, void* filename) -> size_t {
        std::ofstream outStream(reinterpret_cast<const char*>(filename), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
        outStream.write(reinterpret_cast<const char*>(buffer), pocet * velkost);
        return velkost;
    };

    curl_global_init(CURL_GLOBAL_DEFAULT);
    this->curl = curl_easy_init();
}

UrlDownload::~UrlDownload() {
    curl_easy_cleanup(this->curl);
    curl_global_cleanup();
}

UrlDownload& UrlDownload::setNewFilename(const char* filename) {
    this->filename = filename;
    return *this;
}

void UrlDownload::pause() {
    curl_easy_pause(this->curl, CURLPAUSE_RECV);
}

void UrlDownload::resume() {
    curl_easy_pause(this->curl, CURLPAUSE_RECV_CONT);
}

void UrlDownload::stop() {
    curl_easy_pause(this->curl, CURLPAUSE_ALL);
}

std::string UrlDownload::getFilename() const {
    return filename;
}

std::string UrlDownload::getUrl() const {
    return url;
}

