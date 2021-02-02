#pragma once

#include "UrlDownload.hpp"

class HttpDownload : public UrlDownload {
public:
    HttpDownload();
    virtual ~HttpDownload();

    HttpDownload& setUrl(const std::string& );

    void download() override;
};



class HttpsDownload : public HttpDownload {
public:
    HttpsDownload();
    virtual ~HttpsDownload() {};

    HttpsDownload& setUrl(const std::string& );
};

