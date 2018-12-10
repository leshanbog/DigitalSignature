#include "Verification.h"


#include <fstream>
#include <vector>


VerifyCommand::VerifyCommand(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    m_signaturePath(std::move(args.m_signatureFilePath)),
    m_publicKeyPath(std::move(args.m_publicKeyPath)) {}

std::string VerifyCommand::Do()
{
    const auto fileCharacters = ReadFile();
    const Hash fileHash(fileCharacters);

    const auto signature = GetSignature();
    const Signature zeroSignature;
    if (signature == zeroSignature)
        return "Check your signature file!\n";

    const Key publicKey = GetPublicKey();
    const Key zeroKey;
    if (publicKey == zeroKey)
        return "Check your public key!\n";

    // calculate preimage
    const Hash fileHashPreimage = signature.PowMod(publicKey.exp, publicKey.n);

    if (fileHashPreimage == fileHash)
    {
        return "Signature is valid!\nVerification done";
    }
    else
    {
        return "Signature is not valid!\nVerification done";
    }
}

Key VerifyCommand::GetPublicKey()
{
    std::ifstream inputPublicKey(m_publicKeyPath);
    Key pk;
    inputPublicKey >> pk.exp >> pk.n;
    inputPublicKey.close();
    return pk;
}

Signature VerifyCommand::GetSignature()
{
    std::ifstream fileSignature(m_signaturePath);
    Signature signature;
    fileSignature >> signature;
    fileSignature.close();
    return signature;
}

std::vector<unsigned char> VerifyCommand::ReadFile()
{
    try
    {
        std::ifstream fileToSign(m_filePath, std::ios::binary);
        const std::vector<unsigned char> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
        fileToSign.close();
		if (fileCharacters.empty())
			throw std::runtime_error("File is empty!\n");
        return fileCharacters;
    }
    catch (std::bad_alloc& err)
    {
        throw std::runtime_error("File is too big!\n");
    }
}
