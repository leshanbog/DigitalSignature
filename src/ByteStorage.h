#pragma once

#include "InfInt.h"

#include <fstream>
#include <vector>

// TODO: delete
#include <iostream>


typedef uint32_t Byte;

const size_t BYTE_STORAGE_SIZE = 3;


struct ByteStorage
{
    ByteStorage(const std::vector<unsigned char>& stream);
    ByteStorage();

    ByteStorage PowMod(uint32_t exp, uint32_t n) const;
    void SignlePowMod(Byte a, uint32_t exp, uint32_t n, Byte* num) const;

    bool operator==(const ByteStorage& other) const;

    friend std::ofstream& operator<<(std::ofstream& os, const ByteStorage& bs);
    friend std::ifstream& operator>>(std::ifstream& is, ByteStorage& bs);

	Byte m_data[BYTE_STORAGE_SIZE];
};


typedef ByteStorage Signature; 
typedef ByteStorage Hash;
