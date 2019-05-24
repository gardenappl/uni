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
}

#endif // MISC_H
