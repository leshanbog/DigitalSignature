
#include "Signing.h"

#include <fstream>
#include <vector>


Signing::Signing(Arguments& args) :
    m_filePath(std::move(args.m_filePath)),
	PasswordNeeded(std::forward<std::string>(args.m_password)) {}


hash Signing::GetFileHash(const std::vector<byte>& str)
{
	// TODO: implement
	return 5;
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
        return m_filePath.substr(0, end) + "_signed." + m_filePath.substr(end+1, m_filePath.size() - end - 1);
}


Signature Signing::CreateSignature(const std::vector<byte>& fileCharacters, const Key& pk)
{
    Signature signature;
    for (size_t i = 0; i < fileCharacters.size(); ++i)
    {
        // TODO: make real signature
        signature ^= Power(fileCharacters[i], pk); 
    }
    return signature;
}


byte Signing::Power(byte m, const Key& pk)
{
    byte res = m;
    for (uint64_t i = 1; i < pk.exp; ++i)
    {
        // TODO: expand return value
        res = (res * m) % pk.n;
    }
    return res;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


SignNotGenerateCommand::SignNotGenerateCommand(Arguments& args) :
	Signing(args), 
	m_privateKeyPath(std::move(args.m_privateKeyPath)) {}

std::string SignNotGenerateCommand::Do()
{
    // read file
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
    fileToSign.close();

    // read private key
    std::ifstream inputPrivateKey(m_privateKeyPath);
    Key pk;
    inputPrivateKey >> pk.exp >> pk.n;
    inputPrivateKey.close();

    // create signature
    const auto signature = CreateSignature(fileCharacters, pk);

    // save signature
    std::ofstream foutSignature(MakeSignatureFileName());
    foutSignature << signature;
    foutSignature.close();

    return "sign without generating finished";
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


SignAndGenerateCommand::SignAndGenerateCommand(Arguments& args) :
	Signing(args),
    GenerationKeyPair(std::move(args.m_password)) {}

std::string SignAndGenerateCommand::Do()
{
    // TODO: remove copypaste (move duplicate to parent)

    // read file
    std::ifstream fileToSign(m_filePath, std::ios::binary);
    const std::vector<byte> fileCharacters(std::istreambuf_iterator<char>(fileToSign), (std::istreambuf_iterator<char>()));
    fileToSign.close();

    // generate keys
    const Key pk = PerformGeneration();
    

    // create signature
    auto signature = CreateSignature(fileCharacters, pk);

    // save signature
    std::ofstream foutSignature(MakeSignatureFileName());
    foutSignature << signature;
    foutSignature.close();


	return "sign and generating finished";
}

