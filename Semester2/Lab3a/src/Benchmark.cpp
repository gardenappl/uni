#include "Benchmark.h"
#include <chrono>
#include "Point.h"
#include "Utils.h"

namespace sort
{
    void Benchmark::run()
    {
        auto start = std::chrono::system_clock::now();
        auto end = std::chrono::system_clock::now();

        ArrayGenerator<Point> generators[4] =
        {
            Benchmark::new_array_random<Point>,
            Benchmark::new_array_sorted<Point>,
            Benchmark::new_array_sorted_reverse<Point>,
            Benchmark::new_array_almost_sorted<Point>
        };
    }
}
