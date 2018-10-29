#pragma once

#include "GenerationKeyPair.h"
#include "rsa_helper.h"



class Signing : public PasswordNeeded, public ICommand
{
protected:
    Signing(Arguments& args);

    virtual std::string Do() = 0;
    void PerformSigning();
    virtual Key ObtainPrivateKey() = 0;

    std::vector<byte> ReadFile();
    void SaveSignature(const Signature& signature);

    std::string MakeSignatureFileName();

protected:
    std::string m_filePath;
    rsa_helper m_rsaHelper;
};



class SignNotGenerateCommand : public Signing
{
public:
    SignNotGenerateCommand(Arguments& args);
    virtual std::string Do() override;
    virtual Key ObtainPrivateKey() override;

private:
    std::string m_privateKeyPath;
};



class SignAndGenerateCommand : public Signing, public GenerationKeyPair
{
public:
    SignAndGenerateCommand(Arguments& args);
    virtual std::string Do() override;
    virtual Key ObtainPrivateKey() override;
};