## Overview

**Download Manager (Client)** is semester project for the school subject [Principles of Operating Systems](https://vzdelavanie.uniza.sk/vzdelavanie/planinfo.php?kod=274877&lng=sk "Principles of Operating Systems").
The aim of the work was to implement low-level application in C/C++ language with emphasis on multithreading and network protocols implementation.

## User manual
The application is controlled via the command line interface (terminal). \
The commands are inspired by classic Unix-like commands such as ls, **pwd**, **mkdir**, **touch**, **cat** etc. \
(example of application interface) \
![User Manual](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/Use.png "User manual")

[libCURL](https://github.com/curl/curl "libCURL") library is used for network communication, which is an "unwritten standard" in network communication on client side of the C language application, that supports http, https, ftp, sftp and many other protocols.

## Project structure

##### Diagram of download protocols

![UML_URL](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/UML_URL.png "UML_URL")

##### Diagram of download commands (sync/async/future)

![UMLDownload](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/UML_Commands.png "UML_Download")

##### Diagram of filesystem commands

![UMLFileSystem](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/UML_Filesystem.png "UMLFileSystem")

## Commands

Arguments type:
* [required] 
* | optional |

##### ls
	Lists all files and folders in the directory specified as an argument. If no path is specified, the current directory is used.
##### pwd
	Lists the path to the current folder.
##### touch [file_name]
	Creates an empty file.
##### rm [file_name]
	Deletes the file whose path is specified by the argument.
##### mkdir [dir_name]
	Creates an empty directory.
##### rmdir [dir_name]
	Recursively deletes the directory whose path is specified by the argument. If no path is specified, it deletes the current directory.
##### cd [path]
	Move in the directory structure to the address specified by the argument.
##### cat [file_name]
	Lists the contents of the file.
##### cp [file_name] [path]
	Copies the file specified by the argument to the directory specified by the next argument.
##### cpdir [dir_name] [path]
	Recursively copies the directory with all its contents to the location specified by the argument
##### mv [name] [path]
	Moves the file specified by the argument to the directory specified by the next argument.
##### time
	Displays the current local time.
##### http [url] [new_file_name]
	Starts the download via http the protocol. The file specified by the argument as a URL to a file that is also specified by the argument. Communication works on port no. 80.
##### https [url] [new_file_name]
	Starts an encrypted download via the https protocol. The file specified by the argument as a URL to a file that is also specified by the argument. Communication works on port no. 443.
##### ftp [url] [new_file_name] |user:passwoord|
	Starts the download via the ftp protocol. The file specified by the argument as a URL to a file that is also specified by the argument. Communication works on port no. 20.
##### async_http [url] [new_file_name]
	Starts an http download on a new thread. (arguments same as http)
##### async_https [url] [new_file_name]
	Starts an http download on a new thread. (arguments same as https)
##### async_ftp [url] [new_file_name] |user:passwoord|
	Starts an ftp download on a new thread. (arguments same as ftp)
##### future_http [url] [new_file_name] [time (*DD.MM.YYYY hh:mm:ss*)]
	Sets the start of http download to the specified date and time in the format DD.MM.YYYY hh:mm:ss.
##### future_https [url] [new_file_name] [time (*DD.MM.YYYY hh:mm:ss*)]
	Sets the start of https download to the specified date and time in the format DD.MM.YYYY hh:mm:ss.
##### future_ftp [url] [new_file_name] |user:password| [time (*DD.MM.YYYY hh:mm:ss*)]
	Sets the start of ftp download to the specified date and time in the format DD.MM.YYYY hh:mm:ss.
##### url_history
	Lists the history of downloaded files.
##### pause |id (from url_list)|
	Pauses the download. If the argument specifies the download address of the command (we can see it using the url_list command), only one download is paused. If the command is used without an argument, all downloads are paused.
##### resume |id (from url_list)|
	Resumes the paused download. If the argument specifies the download address of the command (we can see it using the url_list command), only one download will start. If the command is used without an argument, all downloads will start.
##### stop |id (from url_list)|
	Stops download. If the argument specifies the download address of the command (we can see it using the url_list command), only one download will stop. If the command is used without an argument, all downloads will stop.
##### url_list
	Displays the current downloads, along with the address of the download command (key in std::map).

## Threads
#### [Thread](https://github.com/GabrielHalvonik/DownloadManagerClient/blob/master/Thread.hpp) class.
This class wraps the standard std::thread and adds functionality for suspending and checking thread status. For proper operation of state changes, a mutex is used, with which two condition_variable work.

![Thread](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/Thread.png "Thread")

This class is used mainly for asynchronous downloads, where we do not want the execution of the download to block the main thread for entering commands.


#### [Timer](https://github.com/GabrielHalvonik/DownloadManagerClient/blob/master/Timer.hpp) class.
The interval between the execution of the main algorithm is defined in milliseconds. If we use the class in "date" mode, the interval between the scheduled time and the current time is calculated. (timeSinceEpoch), and the main algorithm is executed once.
It is also possible to specify the “pauseFunction” and “stopFunction” functions, which can implement the algorithm, for conditional pause / termination of the algorithm. These functions must have a bool return value.
![Timer](https://raw.githubusercontent.com/GabrielHalvonik/DownloadManagerClient/master/manual_res/Timer.png "Timer")
