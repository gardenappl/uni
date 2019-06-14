#include "misc.h"

#include <string>
#include <random>
#include <iostream>

namespace lists
{
    int str_compare(const std::string& s1, const std::string& s2)
    {
        int length_diff = s1.length() - s2.length();
        if(length_diff != 0)
            return length_diff;
        if(s1 == s2)
            return 0;
        return s1 < s2 ? -1 : 1;
    }

    std::string make_random_alphanumeric_string(int length)
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<char> dist('a', 'z');

        std::string string(length, '0');

        for(int i = 0; i < length; i++)
            string[i] = dist(rng);

//        std::cout << string << std::endl;

        return string;
    }
}
