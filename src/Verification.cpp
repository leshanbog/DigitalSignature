#include "Verification.h"


#include <fstream>
#include <vector>


VerifyCommand::VerifyCommand(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    m_signaturePath(std::move(args.m_signatureFilePath)),
    m_publicKeyPath(std::move(args.m_publicKeyPath)) {}

std::string VerifyCommand::Do()
{
    std::cout << "Verify do\n";
    const auto fileCharacters = ReadFile();
    const Hash fileHash(fileCharacters);
    std::cout << "DEBUG: file hash = " << fileHash << '\n';

    const auto signature = GetSignature();
    const auto publicKey = GetPublicKey();

    const auto fileHashPreimage = m_rsaHelper.CalculatePreimage(signature, publicKey);

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

std::vector<byte> VerifyCommand::ReadFile()
{
    try
    {
        std::ifstream fileToSign(m_filePath, std::ios::binary);
        const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
        fileToSign.close();
        return fileCharacters;
    }
    catch (std::bad_alloc& err)
    {
        throw std::runtime_error("File is too big!\n");
    }
}
