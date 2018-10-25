#pragma once

#include <string>


#include "GenerationKeyPair.h"
#include "Base.h"

typedef uint64_t Signature;


class Signing : public PasswordNeeded, public ICommand
{
protected:
    Signing(Arguments& args);
    virtual std::string Do() = 0;
    hash GetFileHash(const std::vector<byte>& str);
    std::string MakeSignatureFileName();
    Signature CreateSignature(const std::vector<byte>& fileCharacters, const Key& pk);
    byte Power(byte c, const Key& pk);

    
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