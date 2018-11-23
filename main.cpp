#include "src/impl.h"


#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <memory>
#include <iostream>




void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if( !enable )
        mode &= ~ENABLE_ECHO_INPUT;
    else
        mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode );

#else
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if( !enable )
        tty.c_lflag &= ~ECHO;
    else
        tty.c_lflag |= ECHO;

    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

std::string GetUserPassword()
{
    SetStdinEcho(false);
    std::cout << "Enter the password phrase:\n";
    std::string password;
    std::cin >> password;

    SetStdinEcho(true);
    std::cout << "\nGood choice!\n";
    return password;
}

bool Parse(int argc, char *argv[], int& command, Arguments& args)
{
    if (argc < 2)
        return false;
    
    if (strcmp(argv[1], "sign") == 0)
    {
        if (argc != 3 && argc != 5)
            return false;
        argc == 3 ? command = 1 : command = 2;
        args.m_filePath = argv[2];
        args.m_password = GetUserPassword();

        if (argc == 5)
        {
            args.m_privateKeyPath = argv[4];
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
        if (argc != 2)
            return false;
        args.m_password = GetUserPassword();
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

// TODO: add concurrency