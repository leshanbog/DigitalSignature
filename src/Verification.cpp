#include "Verification.h"


#include <fstream>
#include <functional>
#include <vector>


VerifyCommand::VerifyCommand(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    m_signaturePath(std::move(args.m_signatureFilePath)),
    m_publicKeyPath(std::move(args.m_publicKeyPath)) {}


std::string VerifyCommand::Do()
{
    std::cout << "Verify do\n";
    const auto fileCharacters = ReadFile();
    const hash fileHash = GetFileHash(fileCharacters);
    std::cout << "DEBUG: file hash = " << fileHash << '\n';

    const auto signature = GetSignature();
    const auto publicKey = GetPublicKey();

    const auto fileHashPreimage = CalculatePreimage(publicKey, signature);

    if (fileHashPreimage != fileHash)
    {
        std::cout << "Signature is not valid!\n";
    }
    else
    {
        std::cout << "Signature is valid!\n";
    }

    return "verification done";
}

Key VerifyCommand::GetPublicKey()
{
    std::ifstream inputPublicKey(m_publicKeyPath);
    Key pk;
    inputPublicKey >> pk.exp >> pk.n;
    return pk;
}


Signature VerifyCommand::GetSignature()
{
    std::ifstream fileSignature(m_signaturePath);
    Signature signature;
    fileSignature >> signature;
    return signature;
}


hash VerifyCommand::GetFileHash(const std::vector<byte>& str)
{
    hash h = 0;
    for (size_t i = 0; i < str.size(); ++i)
    {
        h ^= std::hash<byte>{}(str[i]);
    }
    return h;
}

std::vector<byte> VerifyCommand::ReadFile()
{
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
    fileToSign.close();
    return fileCharacters;
}

hash VerifyCommand::CalculatePreimage(const Key& publicKey, const Signature& signature)
{
    return Power(signature, publicKey);
}


hash VerifyCommand::Power(const Signature& signature, const Key& publicKey)
{
    hash res = signature;
    for (uint32_t i = 1; i < publicKey.exp; ++i)
    {
        res = (res * signature) % publicKey.n;
    }
    return res;
}