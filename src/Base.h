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
    Key(uint32_t _exp = 1, uint32_t _n = 128) :
        exp(_exp), n(_n) {}
    uint32_t exp;
    uint32_t n;
};


class ICommand
{
public:
    virtual std::string Do() = 0;
};
