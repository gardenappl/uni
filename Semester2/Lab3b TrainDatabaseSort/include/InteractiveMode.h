#ifndef INTERACTIVEMODE_H
#define INTERACTIVEMODE_H

#include "TrainDatabase.h"

class InteractiveMode
{
    static void add_train(int id = -1);
    static void add_ticket(StationScheduleDatabase* schedule_db);
    static void search_tickets(StationScheduleDatabase* schedule_db);

public:
    static void run();
};


#endif // INTERACTIVEMODE_H
