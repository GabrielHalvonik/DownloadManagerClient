#pragma once

#include <string>
#include <ctime>

class UrlDownload;

namespace Utils {
    static std::string getCurrentTime() {
        std::string result {};
        std::time_t now;
        std::time(&now);
        char* str = new char[20];
        str[19] = '\0';
        std::tm* info = std::localtime(&now);
        std::strftime(str, 19, "%d.%m.%Y %H:%M:%S", info);
        result = str;
        delete[] str;
        return result;
    }
};

