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

typedef uint32_t hash;
typedef unsigned char byte;
typedef uint32_t Signature;

struct Key
{
    Key(uint32_t _exp = 0, uint32_t _n = 0) :
        exp(_exp), n(_n) {}
    uint32_t exp;
    uint32_t n;
};



class ICommand
{
public:
    virtual std::string Do() = 0;
};