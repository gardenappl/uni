#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "Utils.h"
#include "Point.h"
#include "Sort.h"

namespace sort
{
    template<typename T>
    using ArrayGenerator = T*(*)(int, T(*)());

    template<typename T>
    using SortFunction = void(*)(T*, int, bool);

    class Benchmark
    {
    public:
        static void run();

    private:
        template<typename T>
        static T* new_array_random(int count, T(*element_generator)());
        template<typename T>
        static T* new_array_sorted(int count, T(*element_generator)());
        template<typename T>
        static T* new_array_sorted_reverse(int count, T(*element_generator)());
        template<typename T>
        static T* new_array_almost_sorted(int count, T(*element_generator)());

        template<typename T>
        static void std_sort_test(T* array, int count, bool print_steps = false);
        template<typename T, int S>
        static void optimized_sort_test(T* array, int count, bool print_steps = false);
    };

    template<typename T>
    T* Benchmark::new_array_random(int count, T(*element_generator)())
    {
        T* array = new T[count];
        for(int i = 0; i < count; i++)
        {
            array[i] = element_generator();
        }
        return array;
    }

    template<typename T>
    T* Benchmark::new_array_sorted(int count, T(*element_generator)())
    {
        T* array = new_array_random(count, element_generator);
        std::sort(array, array + count);
        return array;
    }

    template<typename T>
    T* Benchmark::new_array_sorted_reverse(int count, T(*element_generator)())
    {
        T* array = new_array_sorted(count, element_generator);
        std::reverse(array, array + count);
        return array;
    }

    template<typename T>
    T* Benchmark::new_array_almost_sorted(int count, T(*element_generator)())
    {
        T* array = new_array_sorted(count, element_generator);
        sort::print_array(array, 0, count);
        for(int i = 0; i < count / 40; i++)
        {
            int from = std::rand() / (RAND_MAX / count);
            int to = std::rand() / (RAND_MAX / count + 1);

            std::cout << from << ' ' << to << std::endl;
            std::cout << "hi" << std::endl;
            std::swap(array[from], array[to]);
            sort::print_array(array, 0, count);
        }
        return array;
    }

    template<typename T>
    void Benchmark::std_sort_test(T* array, int count, bool print_steps)
    {
        std::sort(array, array + count);
    }

    template<typename T, int S>
    void Benchmark::optimized_sort_test(T* array, int count, bool print_steps)
    {
        sort::merge_sort_optimized(array, count, S, print_steps);
    }
}

#endif // BENCHMARK_H
