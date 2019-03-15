#include "InteractiveMode.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <TrainDatabase.h>



void InteractiveMode::run()
{
    std::cout << "Welcome to the Train Database" << std::endl;
    std::cout << "The interactive part of the program is barebones (and doesn't usually check for out-of-bounds values)" << std::endl;
    std::cout << "Available commands: quit, add, search, delete, print, save-bin, load-bin, save-txt, load-txt" << std::endl;

    StationScheduleDatabase* db = new StationScheduleDatabase;

    std::string command;
    while(true)
    {
        std::cout << "Enter command: ";
        std::cin >> command;
        if(command == "quit")
        {
            exit(0);
        }
        else if(command == "add")
        {
            std::cout << "Add train or ticket? (tr: Train, ti: Ticket)";

            std::string str;
            bool add_train = false;
            while(true)
            {
                bool wrong_choice = false;
                std::cin >> str;
                if(str == "tr")
                    add_train = true;
                else if(str != "ti")
                    wrong_choice = true;

                if(!wrong_choice)
                    break;
            }

            if(add_train)
                InteractiveMode::add_train();
            else
                InteractiveMode::add_ticket(db);
        }
        else if(command == "delete")
        {
            std::cout << "Delete train or ticket? (tr: Train, ti: Ticket)";
            std::string str;
            bool delete_train = false;
            while(true)
            {
                bool wrong_choice = false;
                std::cin >> str;
                if(str == "tr")
                    delete_train = true;
                else if(str != "ti")
                    wrong_choice = true;

                if(!wrong_choice)
                    break;
            }

            if(delete_train)
            {
                std::cout << "Enter train ID: ";
                int id;
                std::cin >> id;

                bool result = GlobalTrainDatabase::delete_train(id);
                if(result)
                {
                    std::cout << "Deleted train #" << id << std::endl;
                }
                else
                {
                    std::cout << "Train #" << id << " does not exist." << std::endl;
                }
            }
            else
            {
                std::cout << "Enter ticket ID: ";
                int id;
                std::cin >> id;

                bool result = db->delete_ticket(id);
                if(result)
                {
                    std::cout << "Deleted ticket #" << id << std::endl;
                }
                else
                {
                    std::cout << "Ticket #" << id << " does not exist." << std::endl;
                }
            }
        }
        else if(command == "print")
        {
            std::cout << std::endl;
            GlobalTrainDatabase::print();
            db->print();
        }
        else if(command == "save-bin")
        {
            std::ofstream file("database.bin", std::ios_base::binary);
            db->save_bin(file);
            file.close();
            std::cout << "Saved to database.bin successfully." << std::endl;
        }
        else if(command == "save-txt")
        {
            std::ofstream file("database.txt");
            db->save_bin(file);
            file.close();
            std::cout << "Saved to database.txt successfully." << std::endl;
        }
        else if(command == "load-bin")
        {
            std::ifstream file("database.bin", std::ios_base::binary);
            //db.clear();
            delete db;
            db = StationScheduleDatabase::load_bin(file);
            file.close();
            std::cout << "Loaded from database.bin successfully." << std::endl;
        }
        else if(command == "load-txt")
        {
            std::ifstream file("database.txt");
            //db.clear();
            delete db;
            db = StationScheduleDatabase::load_txt(file);
            file.close();
            std::cout << "Loaded from database.txt successfully." << std::endl;
        }
        else if(command == "search")
        {
            InteractiveMode::search_tickets(db);
        }
    }
}

void InteractiveMode::search_tickets(StationScheduleDatabase* db)
{
    std::cout << "Select search criteria: (1 = name contains substring, 2 = above certain popularity ratio, 3 = arrival time earlier than X)";

    std::vector<int> search_result;

    while(true)
    {
        int type;
        std::cin >> type;
        if(type == 1)
        {
            std::cout << "Enter search string: ";
            std::string query;
            std::cin >> query;
            search_result = db->find_all_tickets([query](const TicketInfo& ticket)
                { return GlobalTrainDatabase::get_train(ticket.train_id)->name.find(query) != std::string::npos; });
            break;
        }
        else if(type == 2)
        {
            std::cout << "Enter popularity ratio: ";
            float pop_ratio;
            std::cin >> pop_ratio;
            search_result = db->find_all_tickets([pop_ratio](const TicketInfo& ticket)
                { return ticket.get_popularity_ratio() > pop_ratio; });
            break;
        }
        else if(type == 3)
        {
            std::cout << "Enter arrival time (in UNIX format): ";
            time_t arrival_time;
            std::cin >> arrival_time;
            search_result = db->find_all_tickets([arrival_time](const TicketInfo& ticket)
                    { return ticket.arrival_time < arrival_time; });
            break;
        }
    }

    std::cout << "Found " << search_result.size() << " results." << std::endl;
    for(int ticket_id : search_result)
    {
        std::cout << *db->get_ticket(ticket_id) << std::endl;
    }
}

void InteractiveMode::add_train(int id)
{
    if(id == -1)
    {
        std::cout << "Enter ID: ";
        std::cin >> id;
    }

    std::cout << "Which type? (s: Standard, i: International, h: HighSpeed, su: Suburban) ";
    TrainType type;

    while(true)
    {
        bool wrong_type = false;

        std::string str;
        std::cin >> str;
        if(str == "s")
            type = TrainType::Standard;
        else if(str == "i")
            type = TrainType::International;
        else if(str == "h")
            type = TrainType::HighSpeed;
        else if(str == "su")
            type = TrainType::Suburban;
        else
            wrong_type = true;

        if(!wrong_type)
            break;
    }

    std::cout << "Enter name (or \"n\" for empty name, may include spaces): ";
    std::string name = "";
    while(name.empty())
    {
        std::getline(std::cin, name);
    }

    if(name == "n")
        name = "";

    Train train(id, type, name);
    GlobalTrainDatabase::add_train(train);

    std::cout << "Successfully added train." << std::endl << train << std::endl;
}

void InteractiveMode::add_ticket(StationScheduleDatabase* schedule_db)
{
    std::cout << "Enter ticket ID: " << std::endl;
    int ticket_id;
    std::cin >> ticket_id;

    std::cout << "Which train ID? " << std::endl;
    int train_id;
    std::cin >> train_id;

    if(!GlobalTrainDatabase::get_train(train_id))
    {
        std::cout << "This train doesn't exist in the database yet. Adding new train entry..." << std::endl;
        add_train(train_id);
    }

    time_t arrival_time;
    std::cout << "Enter arrival date and time (in UNIX format, haha): ";
    std::cin >> arrival_time;

    time_t departure_time;
    std::cout << "Enter departure date and time (in UNIX format again): ";
    std::cin >> departure_time;

    int tickets_available;
    std::cout << "Enter total amount of available tickets: ";
    std::cin >> tickets_available;

    int tickets_wanted;
    std::cout << "Enter amount of wanted tickets: ";
    std::cin >> tickets_wanted;

    TicketInfo ticket(ticket_id, train_id, arrival_time, departure_time, tickets_wanted, tickets_available);

    std::cout << std::endl << "Ticket added to database: " << std::endl << ticket << std::endl << std::endl;

    schedule_db->add_ticket(ticket);
}
