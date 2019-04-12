#include "DemoMode.h"
#include "IpAddress.h"
#include "iostream"
#include "MyDeque.h"

void DemoMode::run()
{
    IpAddress address(123, 255, 0, 255);
    VectorDeque<IpAddress> vector_deque;

    std::cout << "VectorDeque test: " << std::endl;
    Deque<IpAddress>* ip_deque = &vector_deque;
    ip_deque->append_left(address);
    ip_deque->print();
    ip_deque->append_left(IpAddress(0));
    ip_deque->print();
    ip_deque->append_left(IpAddress(1));
    ip_deque->print();
    ip_deque->append_left(IpAddress(2));
    ip_deque->print();
    ip_deque->pop_right();
    ip_deque->print();
    ip_deque->pop_left();
    ip_deque->print();
    std::cout << std::endl;

    std::cout << "ArrayDeque test: " << std::endl;
    ArrayDeque<IpAddress> array_deque(10);
    for(int i = 0; i < 10; i++)
    {
        array_deque.print();
        if(i % 2)
            array_deque.append_left(IpAddress(i));
        else
            array_deque.append_right(IpAddress(i));
    }
    array_deque.print();

    ip_deque = &array_deque;
    for(int i = 0; i < 10; i++)
    {
        ip_deque->print();
        if(i % 2)
            ip_deque->pop_right();
        else
            ip_deque->pop_left();
    }
    ip_deque->print();
    std::cout << std::endl;


    std::cout << "LinkedDeque test: " << std::endl;
    ip_deque = new LinkedDeque<IpAddress>;
    for(int i = 0; i < 10; i++)
    {
        ip_deque->print();
        if(i % 2)
            ip_deque->append_left(IpAddress(i));
        else
            ip_deque->append_right(IpAddress(i));
    }
    ip_deque->print();
    for(int i = 0; i < 5; i++)
    {
        ip_deque->print();
        if(i % 2)
            ip_deque->pop_right();
        else
            ip_deque->pop_left();
    }
    ip_deque->print();
    std::cout << std::endl;
    delete ip_deque;
}
