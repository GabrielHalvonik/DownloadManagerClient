#include "HttpDownload.hpp"

HttpDownload::HttpDownload() : UrlDownload() {
}

HttpDownload::~HttpDownload() {
}

HttpDownload& HttpDownload::setUrl(const std::string& urlBase) {
    UrlDownload::urlBuilder << "http://www.";
    UrlDownload::urlBuilder << urlBase;
    UrlDownload::url = UrlDownload::urlBuilder.str();
    return *this;
}

void HttpDownload::download() {
    curl_easy_setopt(UrlDownload::curl, CURLOPT_URL, UrlDownload::url.c_str());
    curl_easy_setopt(UrlDownload::curl, CURLOPT_WRITEFUNCTION, UrlDownload::writeFunction);
    curl_easy_setopt(UrlDownload::curl, CURLOPT_WRITEDATA, UrlDownload::filename.c_str());

    curl_easy_perform(UrlDownload::curl);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
}



HttpsDownload::HttpsDownload() : HttpDownload() {
}

HttpsDownload& HttpsDownload::setUrl(const std::string& urlBase) {
    UrlDownload::urlBuilder << "https://www.";
    UrlDownload::urlBuilder << urlBase;
    UrlDownload::url = UrlDownload::urlBuilder.str();
    return *this;
}
