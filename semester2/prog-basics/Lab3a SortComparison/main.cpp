#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "Point.h"
#include "Sort.h"
#include "Utils.h"
#include "DemoMode.h"
#include "Benchmark.h"


int main()
{
    std::srand(std::time(0));


    sort::Benchmark::run();
//    sort::DemoMode::run();
}
