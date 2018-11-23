#pragma once

#include <fstream>


typedef uint32_t byte;

const size_t BYTE_STORAGE_SIZE = 30;


struct ByteStorage
{
    ByteStorage(const std::vector<unsigned char>& stream);
    ByteStorage();

    ByteStorage PowMod(uint32_t exp, uint32_t n) const;

    bool operator==(const ByteStorage& other) const;

    friend std::ofstream& operator<<(std::ofstream& os, const ByteStorage& bs);
    friend std::ifstream& operator>>(std::ifstream& is, ByteStorage& bs);

    byte m_data[BYTE_STORAGE_SIZE];
};


typedef ByteStorage Signature; 
typedef ByteStorage Hash;
