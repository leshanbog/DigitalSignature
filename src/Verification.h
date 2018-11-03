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
    std::vector<unsigned char> ReadFile();
    Key GetPublicKey();

private:
    std::string m_filePath;
    std::string m_signaturePath;
    std::string m_publicKeyPath;
};