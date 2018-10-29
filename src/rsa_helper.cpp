#include "rsa_helper.h"

#include <vector>
#include <functional>



Signature rsa_helper::CreateSignature(const Hash& fileHash, const Key& pk) const
{
    return fileHash.PowMod(pk.exp, pk.n);
}

Hash rsa_helper::CalculatePreimage(const Signature& fileSignature, const Key& pk) const
{
    return fileSignature.PowMod(pk.exp, pk.n);
}