#pragma once


#include "Base.h"

#include <iostream>

typedef InfInt primeNumber;

class PasswordNeeded
{
protected:
    PasswordNeeded(std::string&& pass) : m_password(std::move(pass)) {};
    std::string EncodePrivateKey(const Key& pk);
    Key DecodePrivateKey(const std::string& pk);

private:
    std::string GammaChiper(const std::string key, const std::string& data);
    std::vector<int> GeneratePerm(const std::string& word);
    std::string PermutationChiperDecode(const std::string key, const std::string& data);
    std::string PermutationChiperEncode(const std::string key, const std::string& data);

protected:
    std::string m_password;
};



class GenerationKeyPair : public PasswordNeeded
{
protected:
    GenerationKeyPair(std::string& pass) : 
        m_privateKeyPath("privatekey"),
        m_publicKeyPath("publickey"),
        PasswordNeeded(std::move(pass)),
		m_keySize(12)
    {
		std::cout << "Do you want to change default file names in which keys will be generated?  (y - yes, else no)\n";
		char c;
		std::cin.get(c);
		std::cin.get(c);
		if (c == 'y')
		{
			std::cout << "Write the name of file for private key:\n";
			std::cin >> m_privateKeyPath;
			std::cout << "Write the name of file for public key:\n";
			std::cin >> m_publicKeyPath;
		}
		std::cout << "ok..." << std::endl;
	};

    std::string m_privateKeyPath;
    std::string m_publicKeyPath;
	size_t m_keySize;

    primeNumber GenerateRandomPrimeNumber();
    bool IsPrime(const primeNumber& p);
    InfInt ChoosePublicExponent(InfInt phi);
	InfInt FindPrivateExponent(InfInt publicExponent, InfInt phi);
    Key PerformGenerationAndGetPrivateKey();

   template <typename T>
    T Exgcd(T a, T b, T& x, T& y) {
	if (b == 0) {
		x = 1; y = 9;
		return a;
	}
        T g = Exgcd(b, a%b, x, y);
        T tx = x;
        x = y;
        y = tx - (a/b) * x;
        return g;
    } 

    template<typename T> 
    T Gcd(T a, T b)
    {
        T c;
        while (b != 0)
        {
            c = a % b;
            a = b;
            b = c;
        }
        return a;
    }
};

