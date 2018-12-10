#include "impl.h"


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


GenerateCommand::GenerateCommand(Arguments& args) : 
    GenerationKeyPair(args.m_password) {}

std::string GenerateCommand::Do()
{
    PerformGenerationAndGetPrivateKey();
    return "Generation done";
}


std::string HelpCommand::Do()
{
    return "Commands:\n\tsign <path to file> --pr <path to private key>\n\tverify <path to file> <path to signature> <path to public key>\n\tgenerate\n\thelp\n\nThis console application is\nmade by Alexey Bukhtiyarov\nNRU MPEI, A-13-15\n";
}
