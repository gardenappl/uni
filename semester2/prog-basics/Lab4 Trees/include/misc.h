#ifndef MISC_H
#define MISC_H

#include <iostream>

namespace trees
{
    template<typename Iterator>
    void print_collection(std::ostream& os, Iterator begin, const Iterator& end)
    {
        os << "{ ";
        if(begin != end)
            os << *(begin++);
        while(begin != end)
            os << ", " << *(begin++);
        os << " }" << std::endl;
    }

    std::string make_random_alphanumeric_string(int length);
    bool make_random();
    std::string time_to_string(const time_t& time);
}

#endif // MISC_H
