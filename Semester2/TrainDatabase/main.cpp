#include "TrainDatabase.h"
#include "DemoMode.h"
#include "InteractiveMode.h"
#include "BenchmarkMode.h"



int main()
{
    InteractiveMode::run();
    //run_demo_mode();
    //BenchmarkMode::run(BenchmarkMode::benchmark_action_txt_test, "benchmark-database.bin");
    //BenchmarkMode::run(BenchmarkMode::benchmark_action_txt_test, "benchmark-database.txt");
}
