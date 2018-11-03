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
    //std::cout << "DEBUG: file hash = " << (uint32_t)fileHash.m_data[0] << ' ' << (uint32_t)fileHash.m_data[1] << ' ' << (uint32_t)fileHash.m_data[2] << ' ' << (uint32_t)fileHash.m_data[3] << ' ' << (uint32_t)fileHash.m_data[4] << ' ' <<  '\n';

    const auto signature = GetSignature();
    //std::cout << "DEBUG: file sign = " << (uint32_t)signature.m_data[0] << ' ' << (uint32_t)signature.m_data[1] << ' ' << (uint32_t)signature.m_data[2] << ' ' << (uint32_t)signature.m_data[3] << ' ' << (uint32_t)signature.m_data[4] << ' ' <<  '\n';
    const auto publicKey = GetPublicKey();

    // calculate preimage
    const Hash fileHashPreimage = signature.PowMod(publicKey.exp, publicKey.n);

    //std::cout << "DEBUG: file hash preimage = " << (uint32_t)fileHashPreimage.m_data[0] << ' ' << (uint32_t)fileHashPreimage.m_data[1] << ' ' << (uint32_t)fileHashPreimage.m_data[2] << ' ' << (uint32_t)fileHashPreimage.m_data[3] << ' ' << (uint32_t)fileHashPreimage.m_data[4] << ' ' <<  '\n';
  

    if (fileHashPreimage == fileHash)
    {
        std::cout << "Signature is valid!\n";
    }
    else
    {
        std::cout << "Signature is not valid!\n";
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

std::vector<unsigned char> VerifyCommand::ReadFile()
{
    try
    {
        std::ifstream fileToSign(m_filePath, std::ios::binary);
        const std::vector<unsigned char> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
        fileToSign.close();
        return fileCharacters;
    }
    catch (std::bad_alloc& err)
    {
        throw std::runtime_error("File is too big!\n");
    }
}
