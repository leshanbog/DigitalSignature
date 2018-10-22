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


std::string SignCommand(const Arguments& args);
std::string VerifyCommand(const Arguments& args);
std::string GenerateCommand(const Arguments& args);
std::string HelpCommang();