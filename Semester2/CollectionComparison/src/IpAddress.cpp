#include "IpAddress.h"
#include <iostream>

IpAddress::IpAddress(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4)
: bytes({b1, b2, b3, b4})
{
}

IpAddress::IpAddress(uint32_t value)
: value(value)
{}

std::ostream& operator<<(std::ostream& os, const IpAddress& ip)
{
    os << (int)ip.bytes[0] << '.' << (int)ip.bytes[1] << '.' << (int)ip.bytes[2] << '.' << (int)ip.bytes[3];
    return os;
}
