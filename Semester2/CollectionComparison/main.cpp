#include <iostream>
#include <cstdint>
#include "IpAddress.h"
#include "MyDeque.h"
#include "DemoMode.h"
#include "BenchmarkMode.h"

int main()
{
    BenchmarkMode::run();

    std::cout << "The program has finished. Check memory usage and press any button to continue...";
    std::cin.ignore();

    return 0;
}
