#include "ByteStorage.h"


#include <functional>
#include <vector>
#include <string>
#include <thread>

ByteStorage::ByteStorage()
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<Byte>(0);
}


ByteStorage::ByteStorage(const std::vector<unsigned char>& stream)
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<Byte>(stream.size()+i);

    std::hash<Byte> hasher;
    for (size_t i = 0; i < stream.size(); ++i)
    {
        auto h = hasher(stream[i]);
		m_data[i % BYTE_STORAGE_SIZE] ^= h;
    }
    // hash must be smaller then module (n = p*q)
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
       m_data[i] = ((m_data[i] << 2) >> 2);
}

void ByteStorage::SignlePowMod(Byte a, uint32_t exp, uint32_t n, Byte* num) const
{
    uint64_t power = 1;
    uint64_t factor = a;
    for (uint32_t i = 0; i < exp; ++i)
    {
        power = (power * factor) % n;
    }
    *num = static_cast<Byte>(power);
}

ByteStorage ByteStorage::PowMod(uint32_t exp, uint32_t n) const
{
    ByteStorage ans;
    std::thread threads[BYTE_STORAGE_SIZE];
    auto func = [&](Byte* num, int i) { this->SignlePowMod(this->m_data[i], exp,n,num); };
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
    {
        threads[i] = std::thread(func, &ans.m_data[i], i);
    }
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
    {
        threads[i].join();
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
