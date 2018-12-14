#include "src/impl.h"


#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include <memory>
#include <iostream>
#include <cstring>



void SetStdinEcho(bool enable = true)
{
#ifdef _WIN32
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

std::string GetUserPassword(bool needConfirmation)
{
    SetStdinEcho(false);

    std::string password1, password2;

	if (needConfirmation)
	{
		while (true)
		{
			std::cout << "Enter the password phrase:\n";
			std::cin >> password1;
			std::cout << "Confirm the password phrase:\n";
			std::cin >> password2;
			if (password1 != password2)
			{
				std::cout << "Password phrases does not match! Try again\n";
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		std::cout << "Enter the password phrase:\n";
		std::cin >> password1;
	}

    SetStdinEcho(true);
    std::cout << "\nGood choice!\n";
    return password1;
}

bool Parse(int argc, char *argv[], int& command, Arguments& args)
{
    if (argc < 2)
        return false;
    
    if (strcmp(argv[1], "sign") == 0)
    {
        if (argc != 3 && argc != 5 && argc != 7)
            return false;
        args.m_filePath = argv[2];
		bool prKeyProvided = false;

		if (argc > 3)
		{
			if (strcmp(argv[3], "--pr") == 0)
			{
				args.m_privateKeyPath = argv[4];
				prKeyProvided = true;
			}			
			else
			{
				args.m_numberOfDigits = atoi(argv[4]);
			}

			if (argc == 7)
			{
				if (strcmp(argv[5], "--pr") == 0)
				{
					args.m_privateKeyPath = argv[6];
					prKeyProvided = true;
				}			
				else
				{
					args.m_numberOfDigits = atoi(argv[6]);

				}
			}
		}

		if (args.m_numberOfDigits < 6)
		{
			std::cout << "Number of digits is too small, 6 will be used!\n";
			args.m_numberOfDigits = 6;
		}	
		args.m_password = GetUserPassword(!prKeyProvided);
		prKeyProvided ? command = 2 : command = 1;
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
        if (argc != 2 && argc != 4)
            return false;
		if (argc == 4)
		{
			args.m_numberOfDigits = atoi(argv[3]);
			if (args.m_numberOfDigits < 6)
			{
				std::cout << "Number of digits is too small, 6 will be used!\n";
				args.m_numberOfDigits = 6;
			}
		}
        args.m_password = GetUserPassword(true);
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
	if (argc > 1)
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
		catch (std::exception& e)
		{
			std::cout << "Error:\t" << e.what() << '\n';
		}
	}
	else // Dialog mode (temporary)
	{
		std::string cmd;
		std::cout << "Dialog mode started. Enter the command:\n";
		std::getline(std::cin, cmd);

		while (cmd != "exit")
		{
			if (cmd.empty())
			{
				std::getline(std::cin, cmd);
				continue;
			}
			int argc = 1;
			char **argv = new char*[10];

			int i = 0;
			std::string cur = "";
			while (i < cmd.size())
			{
				while (i < cmd.size() && cmd[i] != ' ' && cmd[i] != '\t')
				{
					cur += cmd[i++];
				}
				argv[argc] = new char[cur.size() + 1];
				std::copy(cur.c_str(), cur.c_str() + cur.size(), argv[argc]);
				argv[argc][cur.size()] = 0;
				argc++;
				while (i < cmd.size() && cmd[i] == ' ' || cmd[i] == '\t')
					++i;
				cur = "";
			}

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
					std::cout << "Wrong commang!\nType \"ds help\" to get help\n";
				}
			}
			catch (std::exception& e)
			{
				std::cout << "Error:" << e.what() << '\n';
			}
			for (int i = 1; i < argc; ++i)
				delete argv[i];
			delete[]argv;
			std::cout <<  "\nEnter next command or 'exit' to exit\n";
			std::getline(std::cin, cmd);
		}

	}
    std::cout << '\n';
    return 0;
}