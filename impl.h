#pragma once

#include <iostream>
#include <string>

class Arguments
{
public:
	std::string m_fileName;
	std::string m_signatureFileName;
	std::string m_privateKeyName;
	std::string m_publicKeyName;
	std::string m_password;
};






class ICommand
{
public:
	virtual std::string Do() = 0;
};

ICommand* CreateCommand(const int& commandNumber, const Arguments& args);

class IPasswordNeeded
{
protected:
	std::string m_password;
};


class GenerationKeyPair : public IPasswordNeeded
{
// generating key pair
};


class SignNotGenerateCommand : public ICommand,  public IPasswordNeeded
{
public:
	SignNotGenerateCommand(const Arguments& args);

};


class SignAndGenerateCommand : public ICommand, public GenerationKeyPair
{


};


class VerifyCommand : public ICommand
{


};


class GenerateCommand : public ICommand, public GenerationKeyPair
{

};


class HelpCommand : public ICommand {
public:
	virtual std::string Do() override;
};
