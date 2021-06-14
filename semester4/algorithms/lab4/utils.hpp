#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

namespace utils
{
    /**
     * ������� ��� ��������� ������ � 2D �������� � �++
     */
    template<typename T>
    T& get2D(T* array, size_t width, size_t x, size_t y)
    {
        return array[y * width + x];
    }
};

#endif // UTILS_H
