#ifndef BENCHMARKMODE_H
#define BENCHMARKMODE_H

#include "TrainDatabase.h"
#include <chrono>
#include <functional>
#include <string>

class BenchmarkMode
{
    public:
        static void run(std::function<void(StationScheduleDatabase*&, std::string)> benchmark_action, std::string file_name);

        static void benchmark_action_binary_test(StationScheduleDatabase*& database, std::string file_name);
        static void benchmark_action_txt_test(StationScheduleDatabase*& database, std::string file_name);

    private:
        static void adjust_n(std::chrono::duration<double> last_duration, int* n);
        static int n_40_percent;

        static void test_search(StationScheduleDatabase* database);
        static int get_file_size(std::string file_name); //probably should put this in a Utilities class but meh...
};

#endif // BENCHMARKMODE_H
