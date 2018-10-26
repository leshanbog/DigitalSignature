
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
    // TODO: implement
    static int i = 0;
    // uint16_t temp[]= {39023, 53129};
    uint16_t temp[]= {5, 7};
    return temp[(i++) % 2];
}



uint32_t GenerationKeyPair::ChoosePublicExponent(uint32_t phi)
{
    return 65537;
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