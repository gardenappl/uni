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
        char input = 0;
        while(input != 'd' && input != 'c')
        {
            std::cout << "Print out coordinates for points (c) or only print the distance from origin (d)?" << std::endl;
            std::cin >> input;
        }
        if(input == 'd')
            sort::Point::print_distance = true;

        std::cout << "Enter array size: ";
        int array_size;
        std::cin >> array_size;



        Point* input_array = new Point[array_size];
        for(int i = 0; i < array_size; i++)
        {
            input_array[i] = Point::make_random();
        }
        Point* test_array = new Point[array_size];

        std::cout << "\n\nInsertion sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::insertion_sort(test_array, array_size, true);

        std::cout << "\n\nQuick sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::quick_sort(test_array, array_size, true);

        std::cout << "\n\nMerge sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::merge_sort(test_array, array_size, true);

        std::cout << "\n\nMerge sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::merge_sort(test_array, array_size, true);

        std::cout << "\n\nMerge sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::merge_sort(test_array, array_size, true);

        std::cout << "\n\nMerge sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::merge_sort(test_array, array_size, true);

        std::cout << "\n\nOptimized sort: (please enter \"small array size\"): ";
        int small_array_size;
        std::cin >> small_array_size;
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        sort::merge_sort_optimized(test_array, array_size, small_array_size, true);

        std::cout << "\n\nstd::sort:\n";
        std::copy(input_array, input_array + array_size, test_array);
        sort::print_array(test_array, 0, array_size);
        std::sort(test_array, test_array + array_size);
        sort::print_array(test_array, 0, array_size);

        delete input_array;
    }
}
