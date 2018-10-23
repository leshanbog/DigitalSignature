#include "impl.h"
#include <fstream>
#include <vector>

using std::vector;

typedef uint64_t hash;
typedef uint64_t key;

hash GetFileHash(const vector<char>& str)
{
	// TODO: implement
	return 5;
}


ICommand* CreateCommand(const int& commandNumber, const Arguments& args)
{
	switch (commandNumber)
	{
		case 0:
			return new HelpCommand();
		case 1:
			return new SignNotGenerateCommand(args);
		case 2:
			return new SignAndGenerateCommand(args);
		case 3:
			return new VerifyCommand(args);
		case 4:
			return new GenerateCommand(args);
	}
	throw std::runtime_error("CreateCommand impossible error");
}




std::string SignCommand(const Arguments& args)
{
    std::ifstream fileToSign(args.m_fileName, std::ios::binary);
    const vector<char> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));

    hash fileHash = GetFileHash(fileCharacters);

    return "sign finished";
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=





std::string HelpCommand::Do()
{
	return "TODO: write help";
}
