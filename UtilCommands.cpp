#include "UtilCommands.hpp"
#include "Utils.hpp"

#include <iostream>

void TimeCommand::execute(const std::string&) {
    std::cout << "\t> " << Utils::getCurrentTime() << std::endl;
}
