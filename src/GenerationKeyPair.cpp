#include "GenerationKeyPair.h"

#include <random>
#include <fstream>
#include <functional>
#include <map>
#include <time.h>


std::string PasswordNeeded::EncodePrivateKey(const Key& pk)
{
    std::string data = std::to_string(pk.exp) + " " + std::to_string(pk.n);
    std::string encodedPass = GammaChiper(m_password, PermutationChiperEncode(m_password, data));
    return encodedPass;
}

Key PasswordNeeded::DecodePrivateKey(const std::string& data)
{
	std::string decodedKey = PermutationChiperDecode(m_password, GammaChiper(m_password, data));
	int spaceIndex = decodedKey.find(" ");
	if (spaceIndex == std::string::npos || spaceIndex == 0 || spaceIndex == decodedKey.size() - 1)
		return { 0,0 };
	Key k;
	try
	{
		k.exp = (uint32_t)stoul(decodedKey.substr(0, spaceIndex));
		k.n = (uint32_t)stoul(decodedKey.substr(spaceIndex + 1, data.size() - spaceIndex - 1));
	}
	catch (...)
	{
		k.exp = 1;
		k.n = 128;
	}
	return k;
}

std::string PasswordNeeded::GammaChiper(const std::string key, const std::string& data)
{   
    srand(std::hash<std::string>{}(key));
    std::string encodedData = data;
    for (int i = 0; i < encodedData.size(); ++i)
    {
        unsigned char c = rand() % 256;
        encodedData[i] ^= c;
    }

    return encodedData;
}

std::vector<int> PasswordNeeded::GeneratePerm(const std::string& word)
{
    std::vector<int> v(word.size());
    std::multimap<char, int> m;
    for (int i = 0; i < word.size(); ++i)
        m.insert(std::make_pair(word[i], i));

    int k = 0;
    for (auto& elem : m)
        v[elem.second] = k++;

    return v;
}

std::string PasswordNeeded::PermutationChiperDecode(const std::string key, const std::string& data)
{
     std::vector<int> perm = GeneratePerm(key);

    std::string decodedData = data;

    for (int i = 0; i < data.size() / perm.size(); ++i)
    {
        for (int j = 0; j < perm.size(); ++j)
        {
            decodedData[i * perm.size() + j] = data[i * perm.size() + perm[j]];
        }
    }

    decodedData.resize(decodedData.find_last_not_of(' ') + 1);

    return decodedData;
}

std::string PasswordNeeded::PermutationChiperEncode(const std::string key, const std::string& data)
{
    std::vector<int> perm = GeneratePerm(key);

    std::string dataToEncode = data;
    dataToEncode.insert(dataToEncode.size(),  (perm.size() - (dataToEncode.size() % perm.size())) % perm.size(), ' ');

    std::string encodedData = dataToEncode;
    for (int i = 0; i < dataToEncode.size() / perm.size(); ++i) 
    {
        for (int j = 0; j < perm.size(); ++j)
        {
            encodedData[i * perm.size() + perm[j]] = dataToEncode[i * perm.size() + j];
        }
    }

    return encodedData;
}


primeNumber GenerationKeyPair::GenerateRandomPrimeNumber()
{
    srand(time(0));
    primeNumber p = rand() % 32767 + 32769;
    while (!IsPrime(p))
    {
        p = rand() % 32767 + 32769;
    }
    return p;
}

bool GenerationKeyPair::IsPrime(const primeNumber& p)
{
    if (p % 2 == 0 || 
        p % 3 == 0 ||
        p % 5 == 0 ||
        p % 7 == 0 ||
        p % 11 == 0 ||
        p % 13 == 0 ||
        p % 17 == 0 ||
        p % 19 == 0 ||
        p % 23 == 0 ||
        p % 29 == 0 ||
        p % 31 == 0 ||
        p % 37 == 0 ||
        p % 41 == 0 ||
        p % 43 == 0)
            return false;
    uint32_t l = sqrt(p) + 1;
    for (uint32_t d = 47; d < l; d += 2)
    {
        if (p % d == 0)
            return false;
    }
    return true;
}

uint32_t GenerationKeyPair::ChoosePublicExponent(uint32_t phi)
{
    //return 65537;
    srand(time(0));
    uint32_t publicExponent = (rand() % (phi - 999) ) + 999;
    while (Gcd(publicExponent, phi) != 1)
    {
        publicExponent = (rand() % (phi - 999) ) + 999;
    }
    return publicExponent;
}


uint32_t GenerationKeyPair::FindPrivateExponent(int64_t publicExponent, int64_t phi)
{
    int64_t x,y;
    if (exgcd(publicExponent, phi, x, y) != 1)
    {
        throw std::runtime_error("Phi and public exponent are not coprime! Impossible situation");
    }
    uint32_t pe = (x%phi + phi) % phi;
    return pe;
}

Key GenerationKeyPair::PerformGenerationAndGetPrivateKey()
{
    // TODO: maybe do concurrent
    primeNumber p = GenerateRandomPrimeNumber();
    primeNumber q = GenerateRandomPrimeNumber();
    while (p == q)
    {
        q = GenerateRandomPrimeNumber();
    }
    module n = p * q;
    uint32_t phi = (p-1)*(q-1);
    uint32_t publicExponent = ChoosePublicExponent(phi);
    uint32_t privateExponent = FindPrivateExponent(publicExponent, phi);

    Key privateKey(privateExponent, n);
    std::ofstream foutPrKey(m_privateKeyPath);
    foutPrKey << EncodePrivateKey(privateKey);
    foutPrKey.close();

    std::ofstream foutPubKey(m_publicKeyPath);
    foutPubKey << publicExponent << ' ' << n;
    foutPubKey.close();

    return privateKey;
}