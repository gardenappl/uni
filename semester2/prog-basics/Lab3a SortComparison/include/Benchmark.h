#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "Utils.h"
#include "Point.h"
#include "Sort.h"
#include <string>

namespace sort
{
    template<typename T>
    struct ArrayGenerator
    {
        T*(*function)(int, T(*)());
        std::string name;
        ArrayGenerator(T*(*function)(int, T(*)()), std::string name)
        : function(function), name(name)
        {}
    };

    template<typename T>
    struct SortFunction
    {
        void(*function)(T*, int, bool);
        std::string name;
        SortFunction(void(*function)(T*, int, bool), std::string name)
        : function(function), name(name)
        {}
    };

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
        for(int i = 0; i < count / 20; i++)
        {
            int from = std::rand() / (RAND_MAX / count + 1);
            int to = std::rand() / (RAND_MAX / count + 1);
            std::swap(array[from], array[to]);
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
