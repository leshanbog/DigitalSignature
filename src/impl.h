#pragma once

#include "Signing.h"
#include "Verification.h"


#include <iostream>
#include <string>


ICommand* CreateCommand(const int& commandNumber, Arguments& args);




class GenerateCommand : public ICommand, public GenerationKeyPair
{
public:
    GenerateCommand(Arguments& args);
    virtual std::string Do() override;
};



class HelpCommand : public ICommand {
public:
    virtual std::string Do() override;
};
