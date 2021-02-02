#pragma once

#include "Commands.hpp"

class TimeCommand : public ICommand  {
public:
    void execute(const std::string&) override;
};
