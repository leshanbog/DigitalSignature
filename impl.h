#pragma once

#include <iostream>
#include <string>

#include "src/Signing.h"


ICommand* CreateCommand(const int& commandNumber, Arguments& args);


class VerifyCommand : public ICommand
{
public:
	VerifyCommand(Arguments& args);
	virtual std::string Do() override;

private:
	std::string m_filePath;
	std::string m_signaturePath;
	std::string m_publicKeyPath;
};


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
