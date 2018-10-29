#pragma once

#include "Base.h"


class rsa_helper
{
public:
    Hash CalculatePreimage(const Signature& fileSignature, const Key& pk) const;
    Signature CreateSignature(const Hash& fileHash, const Key& pk) const;
};