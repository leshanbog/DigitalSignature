#pragma once


#include "Base.h"

#include <string>
// TODO: delete
#include <iostream>

typedef uint16_t primeNumber;
typedef uint32_t module;

class PasswordNeeded
{
protected:
    PasswordNeeded(std::string& pass) : m_password(pass) {};
    std::string EncodePrivateKey(const Key& pk, const std::string& m_password);
    Key DecodePrivateKey(const std::string& pk, const std::string& m_password);

private:
    std::string GammaChiper(const std::string key, const std::string& pass);
    std::vector<int> GeneratePerm(const std::string& word);
    std::string PermutationChiperDecode(const std::string key, const std::string& pass);
    std::string PermutationChiperEncode(const std::string key, const std::string& pass);

protected:
    std::string m_password;
};



class GenerationKeyPair : public PasswordNeeded
{
protected:
    GenerationKeyPair(std::string& pass) : 
        m_privateKeyPath("privatekey"),
        m_publicKeyPath("publickey"),
        PasswordNeeded(pass) {};

    std::string m_privateKeyPath;
    std::string m_publicKeyPath;

    primeNumber GenerateRandomPrimeNumber();
    bool IsPrime(const primeNumber& p);
    uint32_t ChoosePublicExponent(uint32_t phi);
    uint32_t FindPrivateExponent(uint64_t publicExponent, uint64_t phi);
    Key PerformGenerationAndGetPrivateKey();


    template<typename T> 
    T Gcd(T a, T b)
    {
        T c;
        while (b)
        {
            c = a % b;
            a = b;
            b = c;
        }
        return a;
    }


};