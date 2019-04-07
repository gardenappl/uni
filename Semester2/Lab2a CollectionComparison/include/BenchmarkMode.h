#ifndef BENCHMARKMODE_H
#define BENCHMARKMODE_H

#include "MyDeque.h"
#include <chrono>
#include <functional>

template<typename T, template<typename> typename DequeT>
//signature is: void (DequeT<T>* test_queue, T(*element_generator)(), int operation_count. int capacity)
using DequeTestScenario = std::function<void(DequeT<T>*(*)(int), T(*)(), int)>;

class BenchmarkMode
{
public:
    static void run();

    template<typename T, template<typename> typename DequeT>
    static void test_scenario(DequeTestScenario<T, DequeT> scenario, DequeT<T>*(*deque_generator)(int), T(*element_generator)());

private:
    template<typename T, template<typename> typename DequeT>
    static void scenario_one_side(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T, template<typename> typename DequeT>
    static void scenario_two_side(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);
    template<typename T, template<typename> typename DequeT>
    static void scenario_random_operation(DequeT<T>*(*deque_generator)(int), T(*element_generator)(), int test_scale);

    static int n_40_percent;
    static void adjust_n(std::chrono::duration<double> last_duration, int* n);
};

#endif // BENCHMARKMODE_H
