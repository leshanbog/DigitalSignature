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
    const auto publicKey = GetPublicKey();


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
        std::ifstream fileToVerify(m_filePath, std::ios::binary);
        const std::vector<unsigned char> fileCharacters(std::istreambuf_iterator<char>(fileToVerify), (std::istreambuf_iterator<char>()));
        fileToVerify.close();
        return fileCharacters;
    }
    catch (std::bad_alloc& err)
    {
        throw std::runtime_error("File is too big!\n");
    }
}
