#ifndef MISC_H
#define MISC_H

#include <string>
#include <random>
#include <ctime>
#include <cstdlib>

namespace trees
{
    std::string make_random_alphanumeric_string(int length)
    {
        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        static std::random_device rd;
        static std::default_random_engine rng(rd());
        static std::uniform_int_distribution<char> dist(0, sizeof(alphanum) - 1);

        std::string string(length, 0);
        for(int i = 0; i < length; i++)
            string[i] = alphanum[dist(rng)];
        return string;
    }

    bool make_random_bool()
    {
        static std::random_device rd;
        static std::default_random_engine rng(rd());
        static std::uniform_int_distribution<char> dist(0, 1);
        return (bool)dist(rng);
    }

    //pretty sure the compiler optimizes the unnecessary std::string copy that would occur on return
    std::string time_to_string(const time_t& time)
    {
        return std::asctime(std::localtime(&time));
    }
}


#endif // MISC_H
