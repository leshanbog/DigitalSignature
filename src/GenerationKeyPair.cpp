
#include <random>
#include <fstream>
#include <functional>
#include <map>

#include "GenerationKeyPair.h"



std::string PasswordNeeded::EncodePrivateKey(const Key& pk, const std::string& m_password)
{
    std::cout << "pass = " << m_password << "\n";
    std::string key = std::to_string(pk.exp) + ' ' + std::to_string(pk.n);
    return GammaChiper(key, PermutationChiperEncode(key, m_password));
}

Key PasswordNeeded::DecodePrivateKey(const std::string& pk, const std::string& m_password)
{
    std::string decodedKey =  PermutationChiperDecode(pk, GammaChiper(pk, m_password));
    int spaceIndex = decodedKey.find(" ");
    return { (uint32_t)stoul(decodedKey.substr(0, spaceIndex)), (uint32_t)stoul(decodedKey.substr(spaceIndex + 1, pk.size() - spaceIndex - 1))};
}

std::string PasswordNeeded::GammaChiper(const std::string key, const std::string& pass)
{
	srand(std::hash<std::string>{}(key));
	std::string encodedPass = pass;
	for (int i = 0; i < encodedPass.size(); ++i)
	{
		unsigned char c = rand() % 256;
		encodedPass[i] ^= c;
	}

	return encodedPass;
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

std::string PasswordNeeded::PermutationChiperDecode(const std::string key, const std::string& pass)
{
	std::vector<int> k = GeneratePerm(key);

	std::string decodedPass = pass;

	for (int i = 0; i < pass.size() / k.size(); ++i)
	{
		for (int j = 0; j < k.size(); ++j)
		{
			decodedPass[i*k.size() + j] = pass[i*k.size()+k[j]];
		}
	}

	while (decodedPass[decodedPass.size() - 1] == ' ')
		decodedPass.erase(decodedPass.begin() + decodedPass.size() - 1);
	
	return decodedPass;
}

std::string PasswordNeeded::PermutationChiperEncode(const std::string key, const std::string& pass)
{
	std::vector<int> k = GeneratePerm(key);

	std::string password = pass;
	while (password.size() % k.size() != 0)
		password += ' ';

	std::string encodedPass = password;
	for (int i = 0; i < password.size() / k.size(); ++i) 
	{
		for (int j = 0; j < k.size(); ++j)
		{
			encodedPass[i*k.size() + k[j]] = password[i*k.size() + j];
		}
	}

	return encodedPass;
}


primeNumber GenerationKeyPair::GenerateRandomPrimeNumber()
{
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
    uint32_t publicExponent = (rand() % (phi - 999) ) + 999;
    while (Gcd(publicExponent, phi) != 1)
    {
        publicExponent = (rand() % (phi - 999) ) + 999;
    }
    return publicExponent;
}


uint32_t GenerationKeyPair::FindPrivateExponent(uint64_t publicExponent, uint64_t phi)
{
	// use exgcd for finding inverse
    uint64_t d = 2;
    while (((d * publicExponent) % phi) != 1)
    {
        ++d;
    }
    return d;
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
    foutPrKey << EncodePrivateKey(privateKey, m_password);
    foutPrKey.close();

    std::ofstream foutPubKey(m_publicKeyPath);
    foutPubKey << publicExponent << ' ' << n;
    foutPubKey.close();

    return privateKey;
}