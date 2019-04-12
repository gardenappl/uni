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
    static void scenario_one_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T>
    static void scenario_two_side(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T>
    static void scenario_random_operation(Deque<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);

    static int n_40_percent;
    static void adjust_n(std::chrono::duration<double> last_duration, int* n);
};

#endif // BENCHMARKMODE_H
