#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "MyDeque.h"
#include "IpAddress.h"

class InteractiveMode
{
public:
    static void run();

private:
    static Deque<IpAddress>* current_deque;
    static IpAddress read_ip_from_user_input();
};

#endif // INTERACTIVEMODE_H
