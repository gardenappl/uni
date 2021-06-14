#include "PerfectHashSet.h"
#include "HumanResources.h"
#include <string>


//(зкопійовано з минулого семестру)
//variant of the Rabin fingerprint
//the result is 256^n * c[0] + 256^(n-1) * c[1] + 256^(n-2) * c[2] + ... + 256c[n-1] + c[n] (mod 101)
//(where n = length - 1)
int rabin_hash(const char* c, size_t length)
{
    int hash = c[0];
    for(size_t i = 1; i < length; i++)
    {
        hash = (hash * 256) % HASH_MOD;
        hash = (hash + c[i]) % HASH_MOD;
    }
    return hash;
}

template<>
int32_t getHashValue<std::string>(const std::string& object)
{
    return rabin_hash(object.c_str(), object.length());
}

template<>
int32_t getHashValue<Employee>(const Employee& object)
{
//    std::cout << "Hash for " << object.getName() << " = " << getHashValue(object.getName()) << std::endl;
    return getHashValue(object.getName());
}
