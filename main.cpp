#include <memory>

#include "src/impl.h"


bool Parse(int argc, char *argv[], int& command, Arguments& args)
{
    if (argc < 2)
        return false;
    
    if (strcmp(argv[1], "sign") == 0)
    {
        if (argc != 4 && argc != 6)
            return false;
        argc == 4 ? command = 1 : command = 2;
        args.m_filePath = argv[2];
        args.m_password= argv[3];

        if (argc == 6)
        {
            args.m_privateKeyPath = argv[5];
        }
    }
    else if (strcmp(argv[1], "verify") == 0)
    {
        command = 3;
        if (argc != 5)
            return false;
        args.m_filePath = argv[2];
        args.m_signatureFilePath = argv[3];
        args.m_publicKeyPath = argv[4];
    }
    else if (strcmp(argv[1], "generate") == 0)
    {
        command = 4;
        if (argc != 3)
            return false;
        args.m_password = argv[2];
    }
    else if (strcmp(argv[1], "help") != 0)
    {
        return false;
    }
    return true;
}


std::string Execute(const int& command, Arguments& args)
{
    auto cmd = std::unique_ptr<ICommand>(CreateCommand(command, args));
    auto result = cmd->Do();
    return result;
}

int main(int argc, char *argv[])
{
    try
    {
        int commandName = 0;
        Arguments args;

        if (Parse(argc, argv, commandName, args))
        {
            const auto result = Execute(commandName, args);
            std::cout << result;
        }
        else
        {
            std::cout << "Wrong commang!\nType \"ds help\" to get help";
        }
    }
    catch (...)
    {
        std::cout << "Error";
    }
    std::cout << '\n';
    return 0;
}

// TODO: add concurrency, remove coping in constructors