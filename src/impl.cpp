#include "impl.h"

#include <vector>



ICommand* CreateCommand(const int& commandNumber, Arguments& args)
{
    switch (commandNumber)
    {
        case 0:
            return new HelpCommand();
        case 1:
            return new SignAndGenerateCommand(args);
        case 2:
            return new SignNotGenerateCommand(args);
        case 3:
            return new VerifyCommand(args);
        case 4:
            return new GenerateCommand(args);
    }
    throw std::runtime_error("CreateCommand impossible error");
}




//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

GenerateCommand::GenerateCommand(Arguments& args) : 
    GenerationKeyPair(args.m_password) {}

std::string GenerateCommand::Do()
{
    PerformGenerationAndGetPrivateKey();
    return "generation done";
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


std::string HelpCommand::Do()
{
    return "TODO: write help";
}
