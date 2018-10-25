#include "impl.h"

#include <vector>



ICommand* CreateCommand(const int& commandNumber, Arguments& args)
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



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

VerifyCommand::VerifyCommand(Arguments& args) :
	m_filePath(std::move(args.m_filePath)),
	m_signaturePath(std::move(args.m_signatureFilePath)),
	m_publicKeyPath(std::move(args.m_publicKeyPath)) {}

std::string VerifyCommand::Do()
{
	std::cout << "Verify do\n";

	return "verification done";
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

GenerateCommand::GenerateCommand(Arguments& args) : 
	GenerationKeyPair(std::move(args.m_password)) {}

std::string GenerateCommand::Do()
{
	PerformGeneration();
	return "generation done";
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


std::string HelpCommand::Do()
{
	return "TODO: write help";
}
