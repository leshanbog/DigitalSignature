#include "ByteStorage.h"


#include <functional>
#include <vector>
#include <string>

ByteStorage::ByteStorage()
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<byte>(0);
}


ByteStorage::ByteStorage(const std::vector<byte>& stream)
{
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        m_data[i] = static_cast<byte>(0);

    for (size_t i = 0; i < stream.size(); ++i)
    {
        auto h = std::hash<byte>{}(stream[i]);
        m_data[i % BYTE_STORAGE_SIZE] ^= static_cast<byte>(h % 256);
    }
}

ByteStorage ByteStorage::PowMod(uint32_t exp, uint32_t n) const
{
    ByteStorage ans;
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
    {
        uint64_t power = 1;
        uint64_t factor = m_data[i];
        for (uint32_t i = 0; i < exp; ++i)
        {
            power = (power * factor) % n;
        }
        ans.m_data[i] = static_cast<byte>(power % 256);
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


bool ByteStorage::operator!=(const ByteStorage& other) const
{
    return !operator==(other);
}

std::ostream& operator<<(std::ostream& os, const ByteStorage& bs)
{
    std::string s = "";
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        s += bs.m_data[i];
    
    os << s;
    return os;
}

std::istream& operator>>(std::istream& is, ByteStorage& bs)
{
    std::string s;
    is >> s;
    for (size_t i = 0; i < BYTE_STORAGE_SIZE; ++i)
        bs.m_data[i] = s[i];
    return is;
}