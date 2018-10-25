#pragma once

#include <string>

struct Arguments
{
	std::string m_filePath;
	std::string m_signatureFilePath;
	std::string m_privateKeyPath;
	std::string m_publicKeyPath;
	std::string m_password;
};

typedef uint64_t hash;
typedef unsigned char byte;

struct Key
{
    uint64_t exp;
    uint64_t n;
};



class ICommand
{
public:
	virtual std::string Do() = 0;
};