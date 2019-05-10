#ifndef BENCHMARKMODE_H
#define BENCHMARKMODE_H

#include "MyDeque.h"
#include <chrono>
#include <functional>

template<typename T>
//signature is: void (Deque<T>* test_queue, T(*element_generator)(), int operation_count. int capacity)
using DequeTestScenario = std::function<void(Deque<T>*(*)(int), T(*)(), int)>;

class BenchmarkMode
{
public:
    static void run();

    template<typename T>
    static void test_scenario(DequeTestScenario<T> scenario, Deque<T>*(*deque_generator)(int), T(*element_generator)());

private:
    template<typename T>
    static Deque<T>* generate_array_exact_capacity(int test_scale)
    {
        return new ArrayDeque<T>(test_scale);
    }

    template<typename T>
    static Deque<T>* generate_linked_deque(int test_scale)
    {
        return new LinkedDeque<T>;
    }

    template<typename T>
    static Deque<T>* generate_vector_deque(int test_scale)
    {
        return new VectorDeque<T>;
    }
    template<typename T>
    static void scenario_one_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T>
    static void scenario_two_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T>
    static void scenario_random_operation(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);

    static int n_40_percent;
    static void adjust_n(std::chrono::duration<double> last_duration, int* n);
};



template<typename T>
void BenchmarkMode::test_scenario(DequeTestScenario<T> scenario,
        Deque<T>*(*deque_generator)(int), T(*element_generator)())
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;

    int n = 512;
    do
    {
        BenchmarkMode::adjust_n(diff, &n);
        start = std::chrono::system_clock::now();

        scenario(deque_generator, element_generator, n);

        end = std::chrono::system_clock::now();
        diff = end - start;

//        std::cout << DoubleNode<IpAddress>::total << std::endl;
        std::cout << " (" << diff.count() << " s)" << std::endl;
    }while(diff < std::chrono::seconds(5));

    std::cout << "Average operation speed (op/s): " << (int)(n * (std::chrono::seconds(1) / diff)) << std::endl;
}

template<typename T>
void BenchmarkMode::scenario_one_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    Deque<T>* test_deque = deque_generator(test_scale / 2);
    for(int i = 0; i < test_scale / 2; i++)
    {
        test_deque->append_right(element_generator());
    }
    for(int i = 0; i < test_scale / 2; i++)
    {
        test_deque->pop_right();
    }
//    std::cout << test_deque->get_count() << " elements. ";
    std::cout << "Filled and popped " << test_scale / 2 << " entries (" << (test_scale / 2) * 2 << " operations).";
    delete test_deque;
//    std::cout << DoubleNode<T>::total<< ' ';
}

template<typename T>
void BenchmarkMode::scenario_two_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    Deque<T>* test_deque = deque_generator(test_scale / 2);
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
//    std::cout << test_deque->get_count() << " elements. ";
    delete test_deque;
    std::cout << "Filled and popped " << test_scale / 2 << " entries (" << (test_scale / 2) * 2 << " operations).";
}

template<typename T>
void BenchmarkMode::scenario_random_operation(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale)
{
    //int appends = 0;
    Deque<T>* test_deque = deque_generator(test_scale / 2);
    for(int i = 0; i < test_scale; i++)
    {
        int opertaion_type;
        if(test_deque->get_count() == test_scale)
        {
            opertaion_type = std::rand() % 2; //only first two operations available, don't append.
        }
        else if(test_deque->is_empty())
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
                test_deque->pop_left();
                break;
            case 1:
                test_deque->pop_right();
                break;
            case 2:
                test_deque->append_left(element_generator());
                //appends++;
                break;
            case 3:
                test_deque->append_right(element_generator());
                //appends++;
                break;
        }
    }
//    test_deque->print();
//    std::cout << test_deque->get_count() << " elements. ";
    std::cout << "Performed " << test_scale << " operations. (space constraint: " << test_scale / 2 << " entries).";
    //std::cout << appends << std::endl;
    delete test_deque;
}


#endif // BENCHMARKMODE_H
