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
    //std::cout << "File hash " << fileHash.m_data[0] << " " << fileHash.m_data[1] << " " << fileHash.m_data[2] << "\n";
 

    const auto signature = GetSignature();
    //std::cout << "Read signature " << signature.m_data[0] << " " << signature.m_data[1] << " " << signature.m_data[2] << "\n";
    const Signature zeroSignature;
    if (signature == zeroSignature)
        return "Check your signature file!\n";
    const auto publicKey = GetPublicKey();
    //std::cout << "Public key for verification " << publicKey.exp << " " << publicKey.n << "\n";

    // calculate preimage
    const Hash fileHashPreimage = signature.PowMod(publicKey.exp, publicKey.n);
    //std::cout << "FileHash preimage " << fileHashPreimage.m_data[0] << " "<< fileHashPreimage.m_data[1] << " " << fileHashPreimage.m_data[2]<<"\n";

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
