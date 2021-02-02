#pragma once

#include <sstream>
#include "UrlDownload.hpp"

class FtpDownload : public UrlDownload {
public:
    FtpDownload();
    virtual ~FtpDownload();

    FtpDownload& setUrl(const std::string&);
    FtpDownload& setFileToDownload(const std::string&);
    FtpDownload& setUser(const std::string&);

    void download() override;

private:
    std::string user {};
};



class SftpDownload : public FtpDownload {
public:
    SftpDownload();
    virtual ~SftpDownload() = default;

    SftpDownload& setUrl(const std::string&);
};
