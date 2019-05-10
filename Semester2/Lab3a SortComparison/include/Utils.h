#ifndef UTILS_H
#define UTILS_H

#include <iostream>

namespace sort
{
    template<typename T>
    void print_array(T* array, int start, int end)
    {
        std::cout << "{ ";

        if(end > start)
            std::cout << array[start];
        for(int i = start + 1; i < end; i++)
        {
            std::cout << ", " << array[i];
        }
        std::cout << " }";
        std::cout << std::endl;
    }
}

#endif // UTILS_H
