#include "ByteStorage.h"


ByteStorage::ByteStorage()
{
    m_data = 0;
}


ByteStorage::ByteStorage(const std::vector<unsigned char>& stream)
{
    Byte h = static_cast<Byte>(stream.size());

    std::hash<Byte> hasher;
    for (size_t i = 0; i < stream.size(); ++i)
    {
        auto curHash = hasher(stream[i]);
		h ^= curHash;
    }

    m_data = h;
}


ByteStorage ByteStorage::PowMod(InfInt exp, InfInt n) const
{
    ByteStorage ans;

	InfInt power = 1;
	InfInt factor = m_data;
    
	while (exp != 0)
	{
		if (exp % 2 == 0)
		{
			factor = (factor * factor) % n;
			exp /= 2;
		}
		else
		{
			power *= static_cast<InfInt>(factor) % n;
			--exp;
		}
	}
	power %= n;
    ans.m_data = power;

    return ans;
}


bool ByteStorage::operator==(const ByteStorage& other) const
{
    return other.m_data == this->m_data;
}


std::ofstream& operator<<(std::ofstream& os, const ByteStorage& bs)
{
    os << bs.m_data;
    return os;
}


std::ifstream& operator>>(std::ifstream& is, ByteStorage& bs)
{
    is >> bs.m_data;
    return is;
}
