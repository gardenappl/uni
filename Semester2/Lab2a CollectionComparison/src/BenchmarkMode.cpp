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
    std::cout << "Pre-allocated ArrayDeque test, random operations: " << std::endl;
    test_scenario<IpAddress, ArrayDeque>
            (BenchmarkMode::scenario_random_operation<IpAddress, ArrayDeque>,
            generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, ArrayDeque>
            (BenchmarkMode::scenario_one_side<IpAddress, ArrayDeque>,
            generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;

    std::cout << "Pre-allocated ArrayDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, ArrayDeque>
            (BenchmarkMode::scenario_two_side<IpAddress, ArrayDeque>,
            generate_array_exact_capacity, generate_ip_address);
    std::cout << std::endl;


    std::cout << "LinkedDeque test, random operations: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_random_operation<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "LinkedDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_one_side<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "LinkedDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, LinkedDeque>
            (BenchmarkMode::scenario_two_side<IpAddress, LinkedDeque>,
            generate_linked_deque, generate_ip_address);
    std::cout << std::endl;


    std::cout << "VectorDeque test, random operations: " << std::endl;
    test_scenario<IpAddress, VectorDeque>
            (BenchmarkMode::scenario_random_operation<IpAddress, VectorDeque>,
            generate_vector_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, one way fill & clear: " << std::endl;
    test_scenario<IpAddress, VectorDeque>
            (BenchmarkMode::scenario_one_side<IpAddress, VectorDeque>,
            generate_vector_deque, generate_ip_address);
    std::cout << std::endl;

    std::cout << "VectorDeque test, two way fill & clear: " << std::endl;
    test_scenario<IpAddress, VectorDeque>
            (BenchmarkMode::scenario_two_side<IpAddress, VectorDeque>,
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

        BenchmarkMode::adjust_n(diff, &n);
        std::cout << " (" << diff.count() << " s)" << std::endl;
    }while(diff < std::chrono::seconds(5));

    std::cout << "Average operation speed (op/s): " << (int)(n * std::chrono::seconds(1) / diff) << std::endl;
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
void BenchmarkMode::scenario_one_side(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    DequeT<T>* test_deque = deque_generator(test_scale / 2);
    for(int i = 0; i < test_scale / 2; i++)
    {
        test_deque->append_right(element_generator());
    }
    for(int i = 0; i < test_scale / 2; i++)
    {
        test_deque->pop_right();
    }
    std::cout << "Filled and popped " << test_scale / 2 << " entries (" << (test_scale / 2) * 2 << " operations).";
    delete test_deque;
//    std::cout << DoubleNode<T>::total<< ' ';
}

template<typename T, template<typename> typename DequeT>
void BenchmarkMode::scenario_two_side(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    DequeT<T>* test_deque = deque_generator(test_scale / 2);
    for(int i = 0; i < test_scale / 2; i++)
    {
        if(i % 2)
            test_deque->append_right(element_generator());
        else
            test_deque->append_left(element_generator());
    }
    for(int i = 0; i < test_scale / 2; i++)
    {
        if(i % 2)
            test_deque->pop_right();
        else
            test_deque->pop_left();
    }
    delete test_deque;
    std::cout << "Filled and popped " << test_scale / 2 << " entries (" << (test_scale / 2) * 2 << " operations).";
}

template<typename T, template<typename> typename DequeT>
void BenchmarkMode::scenario_random_operation(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    DequeT<T>* test_queue = deque_generator(test_scale / 2);
    for(int i = 0; i < test_scale; i++)
    {
        int opertaion_type;
        if(test_queue->get_count() == test_scale)
        {
            opertaion_type = std::rand() % 2; //only first two operations available, don't append.
        }
        else if(test_queue->is_empty())
        {
            opertaion_type = std::rand() % 2 + 2;
        }
        else
        {
            opertaion_type = std::rand() % 4;
        }

        switch(opertaion_type)
        {
            case 0:
                test_queue->pop_left();
                break;
            case 1:
                test_queue->pop_right();
                break;
            case 3:
                test_queue->append_left(element_generator());
                break;
            case 4:
                test_queue->append_right(element_generator());
                break;
        }
    }
    std::cout << "Performed " << test_scale << " operations. (space constraint: " << test_scale / 2 << " entries).";
    delete test_queue;
}
