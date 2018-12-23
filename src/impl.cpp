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
    GenerationKeyPair(args.m_password, args.m_numberOfDigits) {}

std::string GenerateCommand::Do()
{
    PerformGenerationAndGetPrivateKey();
    return "Generation done";
}


std::string HelpCommand::Do()
{
    return "Commands:\n\tsign <path to file> --pr <path to private key> --l <number of digits in generated primes>\n\tverify <path to file> <path to signature> <path to public key>\n\tgenerate --l <number of digits in generated primes>\n\thelp\n\nThis console application is\nmade by Alexey Bukhtiyarov\nNRU MPEI, A-13-15\n";
}
