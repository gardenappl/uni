#ifndef IPADDRESS_H
#define IPADDRESS_H

#include <cstdint>
#include <iostream>


class IpAddress
{
public:
    IpAddress(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4);
    IpAddress(uint32_t value);

    union
    {
        uint8_t bytes[4];
        uint32_t value;
    };
};


std::ostream& operator<<(std::ostream& os, const IpAddress& ip);

#endif // IPADDRESS_H
