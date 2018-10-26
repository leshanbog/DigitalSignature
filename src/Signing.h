#pragma once

#include <string>


#include "GenerationKeyPair.h"
#include "Base.h"


class Signing : public PasswordNeeded, public ICommand
{
protected:
    Signing(Arguments& args);
    virtual std::string Do() = 0;

    std::vector<byte> ReadFile();
    Signature CreateSignature(const hash& fileHash, const Key& pk);
    void SaveSignature(const Signature& signature);


    hash GetFileHash(const std::vector<byte>& str);
    std::string MakeSignatureFileName();

    hash Power(const hash& fileHash, const Key& pk);
    std::string m_filePath;
};


class SignNotGenerateCommand : public Signing
{
public:
    SignNotGenerateCommand(Arguments& args);
    virtual std::string Do() override;


private:
    std::string m_privateKeyPath;
};


class SignAndGenerateCommand : public Signing, public GenerationKeyPair
{
public:
    SignAndGenerateCommand(Arguments& args);
    virtual std::string Do() override;

};