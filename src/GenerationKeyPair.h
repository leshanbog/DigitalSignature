#pragma once


#include "Base.h"

#include <iostream>
#include <string>

typedef uint32_t primeNumber;
typedef uint64_t module;

class PasswordNeeded
{
protected:
	PasswordNeeded(std::string&& pass) : m_password(pass) {};
    std::string EncodePrivateKey(const Key& pk, const std::string& m_password);
    Key DecodePrivateKey(const std::string& pk, const std::string& m_password);

	std::string m_password;
};



class GenerationKeyPair : public PasswordNeeded
{
protected:
	GenerationKeyPair(std::string&& pass) : 
		m_privateKeyPath("privatekey"),
		m_publicKeyPath("publickey"),
		PasswordNeeded(std::forward<std::string>(pass)) {};

	std::string m_privateKeyPath;
	std::string m_publicKeyPath;

	primeNumber GenerateRandomPrimeNumber();
    uint64_t ChoosePublicExponent(uint64_t phi);
    uint64_t FindPrivateExponent(uint64_t publicExponent, uint64_t phi);
    Key PerformGeneration();


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