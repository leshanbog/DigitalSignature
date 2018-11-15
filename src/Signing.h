#pragma once

#include "GenerationKeyPair.h"



class Signing : public ICommand
{
protected:
    Signing(Arguments& args);

    virtual std::string Do() = 0;
    void PerformSigning();
    virtual Key ObtainPrivateKey() = 0;

    std::vector<unsigned char> ReadFile();
    void SaveSignature(const Signature& signature);

    std::string MakeSignatureFileName();

protected:
    std::string m_filePath;
};



class SignNotGenerateCommand : public Signing, public PasswordNeeded
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