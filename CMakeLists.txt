cmake_minimum_required(VERSION 3.5)

project(SemestralkaPOS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SemestralkaPOS
    main.cpp
    UrlDownload.hpp
    UrlDownload.cpp
    HttpDownload.hpp
    HttpDownload.cpp
    FtpDownload.hpp
    FtpDownload.cpp
    Commands.cpp
    Commands.hpp
    FileSystemCommands.cpp
    FileSystemCommands.hpp
    DownloadCommands.cpp
    DownloadCommands.hpp
    Thread.hpp
    Thread.cpp
    Timer.hpp
    Timer.cpp
    Utils.hpp
    UtilCommands.hpp
    UtilCommands.cpp
)

find_package(CURL REQUIRED)
target_link_libraries(SemestralkaPOS CURL::libcurl)
