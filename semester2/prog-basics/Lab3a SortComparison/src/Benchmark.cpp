#include "Benchmark.h"
#include <chrono>
#include "Point.h"
#include "Utils.h"

namespace sort
{
    void Benchmark::run()
    {
        ArrayGenerator<Point> generators[] =
        {
            ArrayGenerator<Point>(Benchmark::new_array_random<Point>, "Random array"),
            ArrayGenerator<Point>(Benchmark::new_array_sorted<Point>, "Sorted array"),
            ArrayGenerator<Point>(Benchmark::new_array_almost_sorted<Point>, "Almost sorted array"),
            ArrayGenerator<Point>(Benchmark::new_array_sorted_reverse<Point>, "Reverse-sorted array"),
        };

        SortFunction<Point> sort_functions[] =
        {
            SortFunction<Point>(sort::insertion_sort<Point>, "Insertion sort"),
            SortFunction<Point>(sort::quick_sort<Point>, "Quick sort"),
            SortFunction<Point>(sort::merge_sort<Point>, "Merge sort"),
            SortFunction<Point>(Benchmark::optimized_sort_test<Point, 5>, "Merge sort (+ insertion sort for array sizes <= 5)"),
            SortFunction<Point>(Benchmark::optimized_sort_test<Point, 10>, "Merge sort (+ insertion sort for array sizes <= 10)"),
            SortFunction<Point>(Benchmark::optimized_sort_test<Point, 20>, "Merge sort (+ insertion sort for array sizes <= 20)"),
            SortFunction<Point>(Benchmark::optimized_sort_test<Point, 100>, "Merge sort (+ insertion sort for array sizes <= 100)"),
            SortFunction<Point>(Benchmark::std_sort_test<Point>, "std::sort")
        };

        for(const SortFunction<Point>& sort_function : sort_functions)
        {
            for(const ArrayGenerator<Point>& generator : generators)
            {
                std::cout << sort_function.name << ", " << generator.name << "... ";
                std::cout.flush();
                const int count = 100000;
                Point* array = generator.function(count, Point::make_random);

                auto timer_start = std::chrono::system_clock::now();

                sort_function.function(array, count, false);

                auto timer_end = std::chrono::system_clock::now();
                std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count() << " ms" << std::endl;
                delete[] array;
            }
        }
    }
}
