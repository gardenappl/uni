#ifndef SORT_H
#define SORT_H

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <new>
#include "Utils.h"

namespace sort
{
    template<typename T>
    void insertion_sort(T* array, int length, bool print_steps = false)
    {
        for(int i = 0; i < length; i++)
        {
            T item = array[i];
            for(int j = i - 1; j >= 0; j--)
            {
                if(array[j] > item)
                    array[j + 1] = array[j];
                else
                {
                    array[j + 1] = item;
                    if(print_steps)
                        sort::print_array(array, 0, length);
                    goto next_element;
                }
            }
            array[0] = item;

        next_element:;
        }
    }

    //Hoare partition with median-of-three pivot is supposedly one of the best QuickSort implementations.

    namespace detail
    {
        template<typename T>
        int partition_hoare(T* array, int start, int end, bool print_steps = false)
        {
            //median-of-three code snippet:
            int middle = start + (end - start) / 2;
            if(array[middle] < array[start])
                std::swap(array[middle], array[start]);
            if(array[end] < array[start])
                std::swap(array[start], array[end]);
            if(array[middle] < array[end])
                std::swap(array[middle], array[end]);

            T pivot = array[end];

            if(print_steps)
            {
                std::cout << "Partitioning from " << start << " to " << end
                        << ", pivot: " << pivot << std::endl;
            }

            int i = start - 1;
            int j = end + 1;

            while(true)
            {
                do
                    i++;
                while(array[i] < pivot);

                do
                    j--;
                while(array[j] > pivot);

                if(i < j)
                {
                    std::swap(array[i], array[j]);
                }
                else
                {
                    if(print_steps)
                    {
                        std::cout << "Partitioning finished: ";
                        sort::print_array(array, start, end + 1);
                    }
//                    if(j == end)
//                        j--;
                    return j;
                }
            }
        }

        //start and end are inclusive
        template<typename T>
        void quick_sort(T* array, int start, int end, bool print_steps = false)
        {
            if(start > end)
                throw std::invalid_argument("start must be lower than or equal to end");
            if(start == end)
                return;

            int pivot_pos = detail::partition_hoare(array, start, end, print_steps);
            quick_sort(array, start, pivot_pos, print_steps);
            quick_sort(array, pivot_pos + 1, end, print_steps);
            if(print_steps)
            {
                std::cout << "Sorted from " << start << " to " << end << std::endl;
                sort::print_array(array, start, end + 1);
            }
        }
    }

    template<typename T>
    void quick_sort(T* array, int count, bool print_steps = false)
    {
        detail::quick_sort(array, 0, count - 1, print_steps);
    }

    //Bottom-up MergeSort

    namespace detail
    {
        //end is exclusive here
        template<typename T>
        void merge(T* inputArray, T* outputArray, int start1, int start2, int end, bool print_steps = false)
        {
            int i1 = start1;
            int i2 = start2;

            for(int i = start1; i < end; i++)
            {
                if(i2 == end || (inputArray[i1] < inputArray[i2] && i1 != start2))
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

        template<typename T>
        void do_merge_sort(T* array, int count, int start_sub_count, bool print_steps = false)
        {
            T* original_array_ptr = array;
            T* output_array = new T[count];
            if(!output_array)
                throw std::bad_alloc();

            for(int sub_count = start_sub_count; sub_count < count; sub_count *= 2)
            {
                if(print_steps)
                    std::cout << "Merging subcount: " << sub_count << std::endl;
                for(int i = 0; i < count; i += sub_count * 2)
                {
                    detail::merge(array, output_array, i, std::min(count, i + sub_count), std::min(count, i + sub_count * 2));
                }
                if(print_steps)
                    sort::print_array(output_array, 0, count);
                std::swap(array, output_array);
            }
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

    template<typename T>
    void merge_sort(T* array, int count, bool print_steps)
    {
        detail::do_merge_sort(array, count, 1, print_steps);
    }

    template<typename T>
    void merge_sort_optimized(T* array, int count, int small_array_size = 10, bool print_steps = false)
    {
        for(int i = 0; i < count; i += small_array_size)
        {
            if(print_steps)
                std::cout << "Insertion sort between " << i << " and "
                        << (i + std::min(count - i, small_array_size) - 1) << std::endl;

            insertion_sort(array + i, std::min(count - i, small_array_size));
            if(print_steps)
                sort::print_array(array, 0, count);
        }
        detail::do_merge_sort(array, count, small_array_size, print_steps);
    }
}

#endif // SORT_H
