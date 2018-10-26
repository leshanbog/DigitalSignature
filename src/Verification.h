#pragma once

#include "Base.h"

#include <iostream>

// TODO: code duplication (ReadFile, GetFileHash implemented in Signing); 
class VerifyCommand : public ICommand
{
public:
    VerifyCommand(Arguments& args);
    virtual std::string Do() override;

    Signature GetSignature();
    hash GetFileHash(const std::vector<byte>& str);
    std::vector<byte> ReadFile();
    hash CalculatePreimage(const Key& publicKey, const Signature& signature);
    hash Power(const Signature& signature, const Key& publicKey);
    Key GetPublicKey();

private:
    std::string m_filePath;
    std::string m_signaturePath;
    std::string m_publicKeyPath;
};