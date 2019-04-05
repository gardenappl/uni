#include "BenchmarkMode.h"
#include <cstdlib>
#include "IpAddress.h"
#include <chrono>
#include <ctime>


//Robust benchmark code, allows for testing different scenarios with different types of elements and deques.
//Also overcomplicated.


IpAddress generate_ip_address()
{
    return IpAddress((uint32_t)std::rand());
}

template<typename T>
ArrayDeque<T>* generate_array_exact_capacity(int test_scale)
{
    return new ArrayDeque<T>(test_scale);
}

template<typename T>
LinkedDeque<T>* generate_linked_deque(int test_scale)
{
    return new LinkedDeque<T>;
}

template<typename T>
VectorDeque<T>* generate_vector_deque(int test_scale)
{
    return new VectorDeque<T>;
}

void BenchmarkMode::run()
{
    std::srand(std::time(nullptr));

    //could be organized better
    std::cout << "LinkedDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_one_side_test<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "LinkedDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_two_side_test<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_one_side_test<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, ArrayDeque>
            (BenchmarkMode::scenario_two_side_test<IpAddress, ArrayDeque>,
            generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, VectorDeque>
            (BenchmarkMode::scenario_one_side_test<IpAddress, VectorDeque>,
            generate_vector_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, VectorDeque>
            (BenchmarkMode::scenario_two_side_test<IpAddress, VectorDeque>,
            generate_vector_deque, generate_ip_address);
    std::cout << std::endl;
}

template<typename T, template<typename> typename DequeT>
void BenchmarkMode::test_scenario(DequeTestScenario<T, DequeT> scenario,
        DequeT<T>*(*deque_generator)(int), T(*element_generator)())
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;

    int n = 1024;
    do
    {
        start = std::chrono::system_clock::now();

        scenario(deque_generator, element_generator, n);

        end = std::chrono::system_clock::now();
        diff = end - start;

        std::cout << diff.count() << " s, entries tested: " << n << std::endl;
        BenchmarkMode::adjust_n(diff, &n);
    }while(diff < std::chrono::seconds(5));

    std::cout << "Average per 5 seconds: " << (int)(n * std::chrono::seconds(5) / diff) << std::endl;
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

template<typename T, template<typename> typename DequeT>
void BenchmarkMode::scenario_one_side_test(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    DequeT<T>* test_deque = deque_generator(test_scale);
    for(int i = 0; i < test_scale; i++)
    {
        test_deque->append_right(element_generator());
    }
    for(int i = 0; i < test_scale; i++)
    {
        test_deque->pop_right();
    }
    delete test_deque;
//    std::cout << DoubleNode<T>::total<< ' ';
}

template<typename T, template<typename> typename DequeT>
void BenchmarkMode::scenario_two_side_test(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    DequeT<T>* test_deque = deque_generator(test_scale);
    for(int i = 0; i < test_scale; i++)
    {
        if(i % 2)
            test_deque->append_right(element_generator());
        else
            test_deque->append_left(element_generator());
    }
    for(int i = 0; i < test_scale; i++)
    {
        if(i % 2)
            test_deque->pop_right();
        else
            test_deque->pop_left();
    }
    delete test_deque;
}
