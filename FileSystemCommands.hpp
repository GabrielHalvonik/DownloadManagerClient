#pragma once

#include "Commands.hpp"

class ICommandWithPathOption : public ICommand {
public:
    virtual ~ICommandWithPathOption() = default;

    virtual void execute(const std::string& args) override;

protected:
    std::filesystem::path path {};
};


class ICommandWithTwoPathOptions : public ICommand {
public:
    virtual ~ICommandWithTwoPathOptions() = default;

    virtual void execute(const std::string& args) override;

protected:
    std::filesystem::path pathFrom {};
    std::filesystem::path pathTo {};
};

class ListCommand : public ICommandWithPathOption {
public:
    virtual ~ListCommand() = default;

    void execute(const std::string& args) override;
};

class PathCommand : public ICommand {
public:
    virtual ~PathCommand() = default;

    void execute(const std::string& args) override;
};

class RemoveCommand : public ICommandWithPathOption {
public:
    virtual ~RemoveCommand() = default;

    void execute(const std::string& args) override;
};

class RemoveDirectoryCommand : public ICommandWithPathOption {
public:
    virtual ~RemoveDirectoryCommand() = default;

    void execute(const std::string& args) override;
};

class MakeDirectoryCommand : public ICommandWithPathOption {
public:
    virtual ~MakeDirectoryCommand() = default;

    void execute(const std::string& args) override;
};

class TouchFileCommand : public ICommandWithPathOption {
public:
    virtual ~TouchFileCommand() = default;

    void execute(const std::string& args) override;
};

class EnterDirectoryCommand : public ICommandWithPathOption {
public:
    virtual ~EnterDirectoryCommand() = default;

    void execute(const std::string& args) override;
};

class PrintFileCommand : public ICommandWithPathOption {
public:
    virtual ~PrintFileCommand() = default;

    void execute(const std::string& args) override;
};

class CopyCommand : public ICommandWithTwoPathOptions {
public:
    virtual ~CopyCommand() = default;

    void execute(const std::string& args) override;
};

class CopyDirectoryCommand : public ICommandWithTwoPathOptions {
public:
    virtual ~CopyDirectoryCommand() = default;

    void execute(const std::string& args) override;
};

class MoveCommand : public ICommandWithTwoPathOptions {
public:
    virtual ~MoveCommand() = default;

    void execute(const std::string& args) override;
};

