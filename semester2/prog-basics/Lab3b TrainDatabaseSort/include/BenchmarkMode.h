#ifndef BENCHMARKMODE_H
#define BENCHMARKMODE_H

#include "TrainDatabase.h"
#include <chrono>
#include <functional>
#include <string>

class BenchmarkMode
{
    public:
        static void run();

    private:
        static void test_scenario(std::function<void()> scenario);
};

#endif // BENCHMARKMODE_H
