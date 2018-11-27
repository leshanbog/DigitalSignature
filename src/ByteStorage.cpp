#include "ByteStorage.h"


#include <functional>
#include <vector>
#include <string>


ByteStorage::ByteStorage()
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<Byte>(0);
}


ByteStorage::ByteStorage(const std::vector<unsigned char>& stream)
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<Byte>(stream.size()+i);

    for (size_t i = 0; i < stream.size(); ++i)
    {
		m_data[i % BYTE_STORAGE_SIZE] ^= stream[i] + 0x9e3779b9 + (m_data[i % BYTE_STORAGE_SIZE] << 6) + (m_data[i % BYTE_STORAGE_SIZE] >> 2);
    }
    // hash must be smaller then module (n = p*q)
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] >>= 2;
}

ByteStorage ByteStorage::PowMod(uint32_t exp, uint32_t n) const
{
    //TODO: concurrency
    ByteStorage ans;
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
    {
        uint64_t power = 1;
        uint64_t factor = m_data[i];
        for (uint32_t i = 0; i < exp; ++i)
        {
            power = (power * factor) % n;
        }
        ans.m_data[i] = static_cast<Byte>(power);
    }
    return ans;
}

bool ByteStorage::operator==(const ByteStorage& other) const
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        if (this->m_data[i] != other.m_data[i])
            return false;
    return true;
}


std::ofstream& operator<<(std::ofstream& os, const ByteStorage& bs)
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        os << bs.m_data[i] << ' '; 

    return os;
}

std::ifstream& operator>>(std::ifstream& is, ByteStorage& bs)
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        is >> bs.m_data[i];
    return is;
}
