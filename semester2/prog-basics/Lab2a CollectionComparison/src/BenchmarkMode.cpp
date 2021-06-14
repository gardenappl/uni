#include "BenchmarkMode.h"
#include <cstdlib>
#include "IpAddress.h"
#include <chrono>
#include <ctime>

//Robust benchmark code, allows for testing different scenarios with different types of elements and deques.
//Also, overcomplicated.


IpAddress generate_ip_address()
{
    return IpAddress((uint32_t)std::rand());
}

void BenchmarkMode::run()
{
    std::srand(std::time(nullptr));

    //could be organized better
    std::cout << "Pre-allocated ArrayDeque test, random operations: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_random_operation<IpAddress>,
            BenchmarkMode::generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_one_side<IpAddress>,
            BenchmarkMode::generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_two_side<IpAddress>,
            BenchmarkMode::generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;


    std::cout << "LinkedDeque test, random operations: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_random_operation<IpAddress>,
            BenchmarkMode::generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "LinkedDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_one_side<IpAddress>,
            BenchmarkMode::generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "LinkedDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_two_side<IpAddress>,
            BenchmarkMode::generate_linked_deque, generate_ip_address);
    std::cout << std::endl;


    std::cout << "VectorDeque test, random operations: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_random_operation<IpAddress>,
            BenchmarkMode::generate_vector_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_one_side<IpAddress>,
            BenchmarkMode::generate_vector_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress>
            (BenchmarkMode::scenario_two_side<IpAddress>,
            BenchmarkMode::generate_vector_deque, generate_ip_address);
    std::cout << std::endl;
}

int BenchmarkMode::n_40_percent = 0;

void BenchmarkMode::adjust_n(std::chrono::duration<double> last_duration, int* n)
{
    if(last_duration < std::chrono::seconds(2))
    {
        *n *= 2;
    }
    else
    {
        if(!BenchmarkMode::n_40_percent)
            BenchmarkMode::n_40_percent = *n;

        *n += BenchmarkMode::n_40_percent / 4;
    }
}
