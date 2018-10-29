#pragma once

#include <fstream>


typedef unsigned char byte;

const size_t BYTE_STORAGE_SIZE = 1;


struct ByteStorage
{
    ByteStorage(const std::vector<byte>& stream);
    ByteStorage();


    ByteStorage PowMod(uint32_t exp, uint32_t n) const;

    bool operator==(const ByteStorage& other) const;
    bool operator!=(const ByteStorage& other) const;

    friend std::ostream& operator<<(std::ostream& os, const ByteStorage& bs);
    friend std::istream& operator>>(std::istream& is, ByteStorage& bs);

    byte m_data[BYTE_STORAGE_SIZE];
};


typedef ByteStorage Signature; 
typedef ByteStorage Hash;