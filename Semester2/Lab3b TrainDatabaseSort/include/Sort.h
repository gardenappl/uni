#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <new>
#include <functional>
#include <initializer_list>
#include <vector>
#include "Utils.h"

namespace sort
{
    //-1 <=> value1 < value2
    //0 <=> value1 == value2
    //1 <=> value1 > value2
    template<typename T>
    using ComparatorFunction = std::function<int(const T&, const T&)>;

    //false <=> value1 >= value2
    //true <=> value1 < value2
    template<typename T>
    struct ComparatorChain
    {
        std::vector<ComparatorFunction<T>> comparators;
        ComparatorChain(std::initializer_list<ComparatorFunction<T>> comparators)
        : comparators(comparators)
        {}

        bool operator()(const T& value1, const T& value2) const
        {
            for(auto comparator : comparators)
            {
                int comparison_result = comparator(value1, value2);
                if(comparison_result != 0)
                {
//                    if(comparison_result == -1)
//                        std::cout << value1 << " < " << value2 << " return " << std::boolalpha << (comparison_result == -1) << std::endl;
//                    if(comparison_result == 1)
//                        std::cout << value1 << " > " << value2 << " return " << std::boolalpha << (comparison_result == -1) << std::endl;
                    return comparison_result == -1;
                }
            }

//            std::cout << value1 << " == " << value2 << " return false" << std::endl;
            return false;
        }
    };

    template<typename T, typename Compare>
    void insertion_sort(T* array, int length, Compare& comparator)
    {
        for(int i = 0; i < length; i++)
        {
            T item = array[i];
            for(int j = i - 1; j >= 0; j--)
            {
                if(comparator(item, array[j]))
                    array[j + 1] = array[j];
                else
                {
                    array[j + 1] = item;
                    goto next_element;
                }
            }
            array[0] = item;

        next_element:;
        }
    }

    //Bottom-up MergeSort

    namespace detail
    {
        //end is exclusive here
        template<typename T, typename Compare>
        void merge(T* inputArray, T* outputArray, int start1, int start2, int end, Compare& comparator)
        {
            int i1 = start1;
            int i2 = start2;

            for(int i = start1; i < end; i++)
            {
                if(i2 == end || (comparator(inputArray[i1], inputArray[i2]) && i1 != start2))
                {
                    outputArray[i] = inputArray[i1];
                    i1++;
                }
                else
                {
                    outputArray[i] = inputArray[i2];
                    i2++;
                }
            }
        }

        template<typename T, typename Compare>
        void do_merge_sort(T* array, int count, int start_sub_count, Compare& comparator)
        {
            T* original_array_ptr = array;
            T* output_array = new T[count];
            if(!output_array)
                throw std::bad_alloc();

            for(int sub_count = start_sub_count; sub_count < count; sub_count *= 2)
            {
                for(int i = 0; i < count; i += sub_count * 2)
                {
                    detail::merge(array, output_array, i, std::min(count, i + sub_count), std::min(count, i + sub_count * 2), comparator);
                }
                std::swap(array, output_array);
            }
            print_array(output_array, count);
            if(array == original_array_ptr)
            {
                delete output_array;
            }
            else
            {
                std::copy(array, array + count, original_array_ptr);
                delete array;
            }
        }
    }

    template<typename T, typename Compare>
    void merge_sort_optimized(T* array, int count, int small_array_size, Compare& comparator)
    {
        for(int i = 0; i < count; i += small_array_size)
        {
            insertion_sort<T, Compare>(array + i, std::min(count - i, small_array_size), comparator);
        }
        detail::do_merge_sort<T, Compare>(array, count, small_array_size, comparator);
    }
}

#endif // SORT_H
