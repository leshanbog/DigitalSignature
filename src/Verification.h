#pragma once

#include "Base.h"
#include "rsa_helper.h"

#include <iostream>

// TODO: code duplication (ReadFile, GetFileHash implemented in Signing); 
class VerifyCommand : public ICommand
{
public:
    VerifyCommand(Arguments& args);
    virtual std::string Do() override;

    Signature GetSignature();
    std::vector<byte> ReadFile();
    Key GetPublicKey();

private:
    std::string m_filePath;
    std::string m_signaturePath;
    std::string m_publicKeyPath;
    rsa_helper m_rsaHelper;
};