#pragma once

#include "InfInt.h"

#include <fstream>
#include <vector>


typedef uint32_t Byte;

struct ByteStorage
{
    ByteStorage(const std::vector<unsigned char>& stream);
    ByteStorage();

    ByteStorage PowMod(InfInt exp, InfInt n) const;

    bool operator==(const ByteStorage& other) const;

    friend std::ofstream& operator<<(std::ofstream& os, const ByteStorage& bs);
    friend std::ifstream& operator>>(std::ifstream& is, ByteStorage& bs);

	InfInt m_data;
};


typedef ByteStorage Signature; 
typedef ByteStorage Hash;
