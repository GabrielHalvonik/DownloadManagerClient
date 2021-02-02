#include "Commands.hpp"

#include <sstream>

void ICommand::parametrizeCommand(const std::string& argsStr) {
    if (!args.empty())
        args.clear();
    std::istringstream iss(argsStr);
    std::string str {};
    while (std::getline(iss, str, ' ')) {
        args.push_back(str);
    }
}
