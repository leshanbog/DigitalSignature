#include "GenerationKeyPair.h"

#include <random>
#include <fstream>
#include <functional>
#include <map>
#include <time.h>

const static InfInt g_firstPrimes[] = {
2,	    3,	    5,	    7,	    11,	    13,	    17,	    19,	    23,	    29,	    31,	    37,
41,	    43,	    47,	    53,	    59,	    61,	    67,	    71,	    73,	    79,	    83,	    89,
97,	    101,	103,	107,	109,	113,	127,	131,	137,	139,	149,	151,
157,	163,	167,	173,	179,	181,	191,	193,	197,	199,	211,	223,
227,	229,	233,	239,	241,	251,	257,	263,	269,	271,	277,	281,
283,	293,	307,	311,	313,	317,	331,	337,	347,	349,	353,	359,
367,	373,	379,	383,	389,	397,	401,	409,	419,	421,	431,	433,
439,	443,	449,	457,	461,	463,	467,	479,	487,	491,	499,	503,
509,	521,	523,	541,	547,	557,	563,	569,	571,	577,	587,	593,
599,	601,	607,	613,	617,	619,	631,	641,	643,	647,	653,	659,
661,	673,	677,	683,	691,	701,	709,	719,	727,	733,	739,	743,
751,	757,	761,	769,	773,	787,	797,	809,	811,	821,	823,	827,
829,	839,	853,	857,	859,	863,	877,	881,	883,	887,	907,	911,
919,	929,	937,	941,	947,	953,	967,	971,	977,	983,	991,	997,
1009,	1013,	1019,	1021,	1031,	1033,	1039,	1049,	1051,	1061,	1063,	1069,
1087,	1091,	1093,	1097,	1103,	1109,	1117,	1123,	1129,	1151,	1153,	1163,
1171,	1181,	1187,	1193,	1201,	1213,	1217,	1223,	1229,	1231,	1237,	1249,
1259,	1277,	1279,	1283,	1289,	1291,	1297,	1301,	1303,	1307,	1319,	1321,
1327,	1361,	1367,	1373,	1381,	1399,	1409,	1423,	1427,	1429,	1433,	1439,
1447,	1451,	1453,	1459,	1471,	1481,	1483,	1487,	1489,	1493,	1499,	1511,
1523,	1531,	1543,	1549,	1553,	1559,	1567,	1571,	1579,	1583,	1597,	1601,
1607,	1609,	1613,	1619,	1621,	1627,	1637,	1657,	1663,	1667,	1669,	1693,
1697,	1699,	1709,	1721,	1723,	1733,	1741,	1747,	1753,	1759,	1777,	1783,
1787,	1789,	1801,	1811,	1823,	1831,	1847,	1861,	1867,	1871,	1873,	1877,
1879,	1889,	1901,	1907,	1913,	1931,	1933,	1949,	1951,	1973,	1979,	1987,
1993,	1997,	1999,	2003,	2011,	2017,	2027,	2029,	2039,	2053,	2063,	2069 };


std::string PasswordNeeded::EncodePrivateKey(const Key& pk)
{
    std::string data = pk.exp.toString() + " " + pk.n.toString();
    std::string encodedPass = GammaChiper(m_password, PermutationChiperEncode(m_password, data));
    return encodedPass;
}

Key PasswordNeeded::DecodePrivateKey(const std::string& data)
{
	std::string decodedKey = PermutationChiperDecode(m_password, GammaChiper(m_password, data));
	int spaceIndex = decodedKey.find(" ");
	if (spaceIndex == std::string::npos || spaceIndex == 0 || spaceIndex == decodedKey.size() - 1)
		throw std::runtime_error("Password phrase is incorrect");
	Key k;
	try
	{
		k.exp = static_cast<InfInt>(decodedKey.substr(0, spaceIndex));
		k.n = static_cast<InfInt>(decodedKey.substr(spaceIndex + 1, data.size() - spaceIndex - 1));
	}
	catch (...)
	{
		throw std::runtime_error("Password phrase is incorrect");
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
	std::string prime(m_keySize, '0');
	InfInt p;
	do
	{
		std::cout << p << " is not prime(\n";
		for (uint32_t i = 0; i < m_keySize; ++i)
		{
			prime[i] = '0' + (rand() % 10);
		}

		p = prime;
		if (p % 2 == 0)
			p++;
	} while (!IsPrime(p));

    return p;
}

bool GenerationKeyPair::IsPrime(const primeNumber& p)
{
	for (int i = 0; i < 312; ++i)
	{
		if (p % g_firstPrimes[i] == 0)
			return false;
	}

	return true;
    InfInt l = p.intSqrt() + 1;
    for (InfInt d = 2081; d < l; d += 2)
    {
        if (p % d == 0)
            return false;
    }
    return true;
}

InfInt GenerationKeyPair::ChoosePublicExponent(InfInt phi)
{
    return 65537;
	/*
    srand(time(0));
	InfInt publicExponent = (rand() % (phi - 999) ) + 999;
    while (Gcd(publicExponent, phi) != 1)
    {
        publicExponent = (rand() % (phi - 999) ) + 999;
    }

    return publicExponent;
	*/
}


InfInt GenerationKeyPair::FindPrivateExponent(InfInt publicExponent, InfInt phi)
{
	InfInt x,y;
    if (exgcd(publicExponent, phi, x, y) != 1)
    {
        throw std::runtime_error("Phi and public exponent are not coprime! Impossible situation");
    }
	InfInt privateExponent = (x%phi + phi) % phi;

    return privateExponent;
}

Key GenerationKeyPair::PerformGenerationAndGetPrivateKey()
{
    // TODO: maybe do concurrent
    srand(time(0));
    primeNumber p = GenerateRandomPrimeNumber();
    primeNumber q = GenerateRandomPrimeNumber();
    while (p == q)
    {
        q = GenerateRandomPrimeNumber();
    }
    InfInt n = p * q;
    InfInt phi = (p-1)*(q-1);
	InfInt publicExponent = ChoosePublicExponent(phi);
	InfInt privateExponent = FindPrivateExponent(publicExponent, phi);

    Key privateKey(privateExponent, n);
    std::ofstream foutPrKey(m_privateKeyPath, std::ios_base::trunc);
    foutPrKey << EncodePrivateKey(privateKey);
    foutPrKey.close();

    std::ofstream foutPubKey(m_publicKeyPath, std::ios_base::trunc);
    foutPubKey << publicExponent << ' ' << n;
    foutPubKey.close();

    return privateKey;
}