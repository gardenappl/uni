#ifndef MISC_H
#define MISC_H

#include <iostream>

namespace test
{
    const int VARIANT = 1;

    int get_random_int(int min, int max);
    float get_random_float(float min, float max);

    template<typename T>
    void print_array(T* array, int count)
    {
        if(count > 0)
            std::cout << array[0];

        for(int i = 1; i < count; i++)
        {
            std::cout << ", " << array[i];
            std::cout.flush();
        }
    }
}

#endif // MISC_H
