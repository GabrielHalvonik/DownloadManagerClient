#include "FileSystemCommands.hpp"

#include <fstream>
#include <iostream>

void ICommandWithPathOption::execute(const std::string& args) {
    if (!args.empty()) {
        ICommand::parametrizeCommand(args);
        this->path = std::filesystem::path(ICommand::args.front());
    } else {
        this->path = std::filesystem::current_path();
    }
}

void ICommandWithTwoPathOptions::execute(const std::string& args) {
    if (!args.empty()) {
        ICommand::parametrizeCommand(args);
        this->pathFrom = std::filesystem::path(ICommand::args.front());
        this->pathTo = std::filesystem::path(*(++ICommand::args.begin()));
    } else {
        this->pathFrom = std::filesystem::current_path();
        this->pathTo = std::filesystem::current_path();
    }
}

void ListCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    for (auto temp : std::filesystem::directory_iterator(ICommandWithPathOption::path)) {
        std::cout << "\t> " << (--temp.path().end())->string() << "\n";
    }
}

void PathCommand::execute(const std::string& args) {
    static_cast<void>(args);
    std::cout << "\t> " << std::filesystem::current_path().string() << "\n";
}

void RemoveCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::filesystem::remove(ICommandWithPathOption::path);
}

void RemoveDirectoryCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::filesystem::remove_all(ICommandWithPathOption::path);
}

void MakeDirectoryCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::filesystem::create_directory(ICommandWithPathOption::path);
}

void TouchFileCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::ofstream(ICommandWithPathOption::path.string());
}

void EnterDirectoryCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::filesystem::current_path(ICommandWithPathOption::path);
}

void PrintFileCommand::execute(const std::string& args) {
    ICommandWithPathOption::execute(args);
    std::ifstream fin(ICommandWithPathOption::path, std::ios_base::ate | std::ios_base::binary);
    size_t size = fin.tellg();
    char* content = new char[size];
    fin.seekg(0);
    fin.read(content, size);
    std::cout << content << "\n";
    delete[] content;
}

void CopyCommand::execute(const std::string& args) {
    ICommandWithTwoPathOptions::execute(args);
    std::filesystem::copy_file(ICommandWithTwoPathOptions::pathFrom, ICommandWithTwoPathOptions::pathTo, std::filesystem::copy_options::overwrite_existing);
}

void CopyDirectoryCommand::execute(const std::string& args) {
    ICommandWithTwoPathOptions::execute(args);
    std::filesystem::copy(ICommandWithTwoPathOptions::pathFrom, ICommandWithTwoPathOptions::pathTo, std::filesystem::copy_options::recursive);
}

void MoveCommand::execute(const std::string& args) {
    ICommandWithTwoPathOptions::execute(args);
    std::filesystem::rename(ICommandWithTwoPathOptions::pathFrom, ICommandWithTwoPathOptions::pathTo);
}
