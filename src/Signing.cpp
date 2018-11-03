#include "Signing.h"


#include <fstream>
#include <vector>
// TODO: delete iostream
#include <iostream>




Signing::Signing(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    PasswordNeeded(args.m_password) {}

std::string Signing::MakeSignatureFileName()
{
    std::string extension = "";
    int end = m_filePath.size() - 1;
    while (end >= 0 && m_filePath[end] != '.')
        --end;
    if (end < 0)
        return m_filePath + "_signature";
    else
        return m_filePath.substr(0, end) + "_signature";
}

std::vector<unsigned char> Signing::ReadFile()
{
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<unsigned char> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
    fileToSign.close();
    std::cout << "file size = " << fileCharacters.size() << '\n';
    return fileCharacters;
}

void Signing::SaveSignature(const Signature& signature)
{
    std::ofstream foutSignature(MakeSignatureFileName());
    foutSignature << signature;
    foutSignature.close();
}

void Signing::PerformSigning()
{
    // TODO: reading file + calculating hash can be performed concurrently with obtaining key
    const auto fileCharacters = ReadFile();
    const Hash fileHash(fileCharacters);
    //std::cout << "DEBUG: file hash = " << (uint32_t)fileHash.m_data[0] << ' ' << (uint32_t)fileHash.m_data[1] << ' ' << (uint32_t)fileHash.m_data[2] << ' ' << (uint32_t)fileHash.m_data[3] << ' ' << (uint32_t)fileHash.m_data[4] << ' ' <<  '\n';

    Key pk = ObtainPrivateKey();
    
    // creating signature
    const Signature signature = fileHash.PowMod(pk.exp, pk.n);
    //std::cout << "DEBUG: file sign = " << (uint32_t)signature.m_data[0] << ' ' << (uint32_t)signature.m_data[1] << ' ' << (uint32_t)signature.m_data[2] << ' ' << (uint32_t)signature.m_data[3] << ' ' << (uint32_t)signature.m_data[4] << ' ' <<  '\n';
    SaveSignature(signature);
}




SignNotGenerateCommand::SignNotGenerateCommand(Arguments& args) :
    Signing(args), 
    m_privateKeyPath(std::move(args.m_privateKeyPath)) {}

std::string SignNotGenerateCommand::Do()
{ 
    PerformSigning();
    return "sign without generating finished";
}

Key SignNotGenerateCommand::ObtainPrivateKey()
{
    std::ifstream inputPrivateKey(m_privateKeyPath);
    Key pk;
    inputPrivateKey >> pk.exp >> pk.n;
    inputPrivateKey.close();
    return pk;
}




SignAndGenerateCommand::SignAndGenerateCommand(Arguments& args) :
    Signing(args),
    GenerationKeyPair(args.m_password) {}

std::string SignAndGenerateCommand::Do()
{
    PerformSigning();
    return "sign and generating finished";
}

Key SignAndGenerateCommand::ObtainPrivateKey()
{
    return PerformGenerationAndGetPrivateKey();
}