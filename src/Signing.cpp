#include "Signing.h"


#include <fstream>
#include <vector>

// TODO: delete
#include <iostream>


Signing::Signing(Arguments& args) :
    m_filePath(std::move(args.m_filePath))
{
    m_signaturePath = MakeSignatureFileName();
    std::cout << "Do you want to change signature file name from default " << m_signaturePath << "? ( y - yes, else no)\n";
	char c;
	std::cin.get(c);
	std::cin.get(c);
	if (c == 'y')
	{
		std::cout << "Write the name of signature file:\n";
		std::cin >> m_signaturePath;
	}
	std::cout << std::endl;
}

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

void Signing::SaveSignature(const Signature& signature)
{
    std::ofstream foutSignature(m_signaturePath, std::ios_base::trunc);
    foutSignature << signature;
    foutSignature.close();
}

void Signing::PerformSigning()
{
    // TODO: reading file + calculating hash can be performed concurrently with obtaining key
    const auto fileCharacters = ReadFile();
    const Hash fileHash(fileCharacters);
 
    Key pk = ObtainPrivateKey();
    
    // creating signature
    const Signature signature = fileHash.PowMod(pk.exp, pk.n);
 
    SaveSignature(signature);
}


SignNotGenerateCommand::SignNotGenerateCommand(Arguments& args) :
    Signing(args), 
    PasswordNeeded(std::move(args.m_password)),
    m_privateKeyPath(std::move(args.m_privateKeyPath)) {}

std::string SignNotGenerateCommand::Do()
{ 
    PerformSigning();
    return "Sign without generating finished";
}

Key SignNotGenerateCommand::ObtainPrivateKey()
{
    std::ifstream inputPrivateKey(m_privateKeyPath);
    std::string encodedPrivateKey((std::istreambuf_iterator<char>(inputPrivateKey)), std::istreambuf_iterator<char>());	
    inputPrivateKey.close();
    auto pk = DecodePrivateKey(encodedPrivateKey);
    return pk;
}


SignAndGenerateCommand::SignAndGenerateCommand(Arguments& args) :
    Signing(args),
    GenerationKeyPair(args.m_password) {}

std::string SignAndGenerateCommand::Do()
{
    PerformSigning();
    return "Sign and generating finished";
}

Key SignAndGenerateCommand::ObtainPrivateKey()
{
    return PerformGenerationAndGetPrivateKey();
}