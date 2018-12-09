#pragma once

#include "ByteStorage.h"

#include <string>

struct Arguments
{
    std::string m_filePath;
    std::string m_signatureFilePath;
    std::string m_privateKeyPath;
    std::string m_publicKeyPath;
    std::string m_password;
};


struct Key
{
    Key(InfInt _exp = 1, InfInt _n = 128) :
        exp(_exp), n(_n) {}
	InfInt exp;
	InfInt n;
};


class ICommand
{
public:
    virtual std::string Do() = 0;
};
