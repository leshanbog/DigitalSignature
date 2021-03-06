#pragma once

#include "ByteStorage.h"

#include <string>

struct Arguments
{
    Arguments() : m_numberOfDigits(6) {}
    std::string m_filePath;
    std::string m_signatureFilePath;
    std::string m_privateKeyPath;
    std::string m_publicKeyPath;
    std::string m_password;
    int m_numberOfDigits;
};

struct Key
{
    Key(InfInt _exp = 1, InfInt _n = 128) :
        exp(_exp), n(_n) {}
	InfInt exp;
	InfInt n;
    bool operator==(const Key& other) const
    {
        return this->exp == other.exp && this->n == other.n;
    }
};


class ICommand
{
public:
    virtual std::string Do() = 0;
};
