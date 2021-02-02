#pragma once

#include <filesystem>
#include <list>
#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;

    virtual void execute(const std::string& args = {}) = 0;

    void parametrizeCommand(const std::string& argsStr);

protected:
    std::list<std::string> args;
};
