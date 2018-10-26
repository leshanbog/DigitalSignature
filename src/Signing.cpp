
#include "Signing.h"

#include <fstream>
#include <vector>
#include <functional>


Signing::Signing(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
    PasswordNeeded(std::move(args.m_password)) {}


hash Signing::GetFileHash(const std::vector<byte>& str)
{
    hash h = 0;
    for (size_t i = 0; i < str.size(); ++i)
    {
        h ^= std::hash<byte>{}(str[i]);
    }
    return h;
}


std::string Signing::MakeSignatureFileName()
{
    std::string extension = "";
    int end = m_filePath.size() - 1;
    while (end >= 0 && m_filePath[end] != '.')
        --end;
    if (end < 0)
        return m_filePath + "_signed";
    else
        return m_filePath.substr(0, end) + "_signature";
}


Signature Signing::CreateSignature(const hash& fileHash, const Key& pk)
{
    return Power(fileHash, pk);
}


hash Signing::Power(const hash& fileHash, const Key& pk)
{
    hash res = fileHash;
    for (uint32_t i = 1; i < pk.exp; ++i)
    {
        // TODO: expand return value
        res = (res * fileHash) % pk.n;
    }
    return res;
}

std::vector<byte> Signing::ReadFile()
{
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
    fileToSign.close();
    return fileCharacters;
}

void Signing::SaveSignature(const Signature& signature)
{
    std::ofstream foutSignature(MakeSignatureFileName());
    foutSignature << signature;
    foutSignature.close();
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


SignNotGenerateCommand::SignNotGenerateCommand(Arguments& args) :
    Signing(args), 
    m_privateKeyPath(std::move(args.m_privateKeyPath)) {}

std::string SignNotGenerateCommand::Do()
{ 
    const auto fileCharacters = ReadFile();
    const hash fileHash = GetFileHash(fileCharacters);
    std::cout << "DEBUG: file hash = " << fileHash << '\n';

    // read private key
    std::ifstream inputPrivateKey(m_privateKeyPath);
    Key pk;
    inputPrivateKey >> pk.exp >> pk.n;
    inputPrivateKey.close();


    const auto signature = CreateSignature(fileHash, pk);
    SaveSignature(signature);

    return "sign without generating finished";
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


SignAndGenerateCommand::SignAndGenerateCommand(Arguments& args) :
    Signing(args),
    GenerationKeyPair(std::move(args.m_password)) {}

std::string SignAndGenerateCommand::Do()
{
    // TODO: code duplication, again...
    const auto fileCharacters = ReadFile();
    const hash fileHash = GetFileHash(fileCharacters);

    const Key pk = PerformGenerationAndGetPrivateKey();
    
    const auto signature = CreateSignature(fileHash, pk);
    SaveSignature(signature);

    return "sign and generating finished";
}

