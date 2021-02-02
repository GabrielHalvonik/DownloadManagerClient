#include "FtpDownload.hpp"

FtpDownload::FtpDownload() : UrlDownload() {
}

FtpDownload::~FtpDownload() {
}

FtpDownload& FtpDownload::setUrl(const std::string& urlBase) {
    UrlDownload::urlBuilder << "ftp://";
    UrlDownload::urlBuilder << urlBase;
    UrlDownload::url = UrlDownload::urlBuilder.str();
    return *this;
}

FtpDownload& FtpDownload::setFileToDownload(const std::string& file) {
    UrlDownload::urlBuilder << "/" << file;
    UrlDownload::url = UrlDownload::urlBuilder.str();
    return *this;
}

FtpDownload& FtpDownload::setUser(const std::string& user) {
    this->user = user;
    return *this;
}

void FtpDownload::download() {
    curl_easy_setopt(UrlDownload::curl, CURLOPT_URL, UrlDownload::url.c_str());
    curl_easy_setopt(UrlDownload::curl, CURLOPT_WRITEFUNCTION, UrlDownload::writeFunction);
    curl_easy_setopt(UrlDownload::curl, CURLOPT_WRITEDATA, UrlDownload::filename.c_str());

    if (!this->user.empty()) {
        curl_easy_setopt(UrlDownload::curl, CURLOPT_USERPWD, this->user.c_str());
    }

    //#ifndef DISABLE_SSH_AGENT
    //    /* We activate ssh agent. For this to work you need
    //       to have ssh-agent running (type set | grep SSH_AGENT to check) or
    //       pageant on Windows (there is an icon in systray if so) */
    //    curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_AGENT);
    //#endif

//    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_perform(UrlDownload::curl);
}




SftpDownload::SftpDownload() : FtpDownload() {
}

SftpDownload& SftpDownload::setUrl(const std::string& urlBase) {
    UrlDownload::urlBuilder << "sftp://";
    UrlDownload::urlBuilder << urlBase;
    UrlDownload::url = UrlDownload::urlBuilder.str();
    return *this;
}
