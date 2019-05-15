#ifndef UTILS_H
#define UTILS_H

#include <iostream>

template<typename T>
void print_array(T* array, int count)
{
    std::cout << "{ ";

    if(count != 0)
        std::cout << array[0];
    for(int i = 1; i < count; i++)
    {
        std::cout << ", " << array[i];
    }
    std::cout << " }";
    std::cout << std::endl;
}

#endif // UTILS_H

