#include <iostream>
#include "DemoMode.h"
#include "Point.h"
#include "Sort.h"
#include <algorithm>
#include <iterator>

namespace sort
{
    void DemoMode::run()
    {
        std::cout << "Enter array size: ";
        int array_size;
        std::cin >> array_size;



        Point* input_array = new Point[array_size];
        for(int i = 0; i < array_size; i++)
        {
            input_array[i] = Point::get_random();
        }
        Point* array = new Point[array_size];

        std::cout << "\n\nInsertion sort:\n";
        std::copy(input_array, input_array + array_size, array);
        sort::print_array(array, 0, array_size);
        sort::insertion_sort(array, array_size, true);

        std::cout << "\n\nQuick sort:\n";
        std::copy(input_array, input_array + array_size, array);
        sort::print_array(array, 0, array_size);
        sort::quick_sort(array, array_size, true);

        std::cout << "\n\nMerge sort:\n";
        std::copy(input_array, input_array + array_size, array);
        sort::print_array(array, 0, array_size);
        sort::merge_sort(array, array_size, true);

        std::cout << "\n\nOptimized sort: (please enter \"small array size\"): ";
        int small_array_size;
        std::cin >> small_array_size;
        std::copy(input_array, input_array + array_size, array);
        sort::print_array(array, 0, array_size);
        sort::merge_sort_optimized(array, array_size, small_array_size, true);

        std::cout << "\n\nstd::sort:\n";
        std::copy(input_array, input_array + array_size, array);
        sort::print_array(array, 0, array_size);
        std::sort(array, array + array_size);
        sort::print_array(array, 0, array_size);

        delete input_array;
    }
}
