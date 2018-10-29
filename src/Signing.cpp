#include "Signing.h"


#include <fstream>
#include <vector>
// TODO: delete iostream
#include <iostream>




Signing::Signing(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    PasswordNeeded(std::move(args.m_password)) {}

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

std::vector<byte> Signing::ReadFile()
{
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
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
    const auto fileCharacters = ReadFile();
    const Hash fileHash(fileCharacters);
    std::cout << "DEBUG: file hash = " << fileHash << '\n';

    Key pk = ObtainPrivateKey();

    const auto signature = m_rsaHelper.CreateSignature(fileHash, pk);
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
    GenerationKeyPair(std::move(args.m_password)) {}

std::string SignAndGenerateCommand::Do()
{
    PerformSigning();
    return "sign and generating finished";
}

Key SignAndGenerateCommand::ObtainPrivateKey()
{
    return PerformGenerationAndGetPrivateKey();
}