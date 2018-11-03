
#include <random>
#include <fstream>

#include "GenerationKeyPair.h"



std::string PasswordNeeded::EncodePrivateKey(const Key& pk, const std::string& m_password)
{
    return std::to_string(pk.exp) + ' ' + std::to_string(pk.n);
}

Key PasswordNeeded::DecodePrivateKey(const std::string& pk, const std::string& m_password)
{
    int spaceIndex = pk.find(" ");
    return { (uint32_t)stoul(pk.substr(0, spaceIndex)), (uint32_t)stoul(pk.substr(spaceIndex + 1, pk.size() - spaceIndex - 1))};
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

    std::cout << p << ' ' << q  << '\n';
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