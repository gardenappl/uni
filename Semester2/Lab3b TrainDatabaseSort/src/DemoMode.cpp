#include "DemoMode.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "TrainDatabase.h"

void run_demo_mode()
{
    GlobalTrainDatabase::add_train(Train(1, TrainType::HighSpeed, "Hype Train"));
    GlobalTrainDatabase::add_train(Train(10, TrainType::International));

    StationScheduleDatabase station1;

    station1.add_ticket(TicketInfo(1, 1, time(nullptr) - 10000, time(nullptr), 0, 6));
    station1.add_ticket(TicketInfo(2, 10, time(nullptr) + 25000, time(nullptr) + 50000, 10, 9));
    station1.add_ticket(TicketInfo(3, 10, time(nullptr) + 1, time(nullptr) + 2, 1, 9));
    station1.add_ticket(TicketInfo(4, 1, time(nullptr), time(nullptr) + 10000, 9, 9));

    std::cout << "============ All tickets: ============" << std::endl;
    station1.print();

    std::cout << "============ Tickets with >50% Popularity Ratio(tm): ============" << std::endl;
    std::vector<int> ticket_ids = station1.find_all_tickets([](const TicketInfo& ticket) { return ticket.get_popularity_ratio() > 0.5f; });
    for(int ticket_id : ticket_ids)
    {
        std::cout << *station1.get_ticket(ticket_id) << std::endl << std::endl;
    }

    std::cout << "============ Save and load train in text mode (trains.txt): ============" << std::endl;

    std::ofstream output_stream("trains.txt");
    GlobalTrainDatabase::get_train(1)->save_txt(output_stream);
    output_stream.close();

    std::ifstream input_stream("trains.txt");
    std::cout << Train::load_txt(input_stream) << std::endl << std::endl;
    input_stream.close();

    std::cout << "============ Save and load train in binary mode (trains.bin): ============" << std::endl;

    output_stream.open("trains.bin", std::ios_base::binary);
    GlobalTrainDatabase::get_train(1)->save_bin(output_stream);
    output_stream.close();

    input_stream.open("trains.bin", std::ios_base::binary);
    std::cout << Train::load_bin(input_stream) << std::endl << std::endl;
    input_stream.close();

    std::cout << "============ Save and load a ticket in text mode (ticket.txt): ============" << std::endl;

    output_stream.open("ticket.txt");
    station1.get_ticket(ticket_ids[1])->save_txt(output_stream);
    output_stream.close();

    input_stream.open("ticket.txt");
    std::cout << TicketInfo::load_txt(input_stream) << std::endl << std::endl;
    input_stream.close();

    std::cout << "============ Save and load a ticket in binary mode (ticket.bin): ============" << std::endl;

    output_stream.open("ticket.bin", std::ios_base::binary);
    station1.get_ticket(ticket_ids[1])->save_bin(output_stream);
    output_stream.close();

    input_stream.open("ticket.bin", std::ios_base::binary);
    std::cout << TicketInfo::load_bin(input_stream) << std::endl << std::endl;
    input_stream.close();

    std::cout << "============ Save and load database in text mode (database.txt): ============" << std::endl;

    output_stream.open("database.txt");
    station1.save_txt(output_stream);
    output_stream.close();

    GlobalTrainDatabase::clear();
    input_stream.open("database.txt");
    StationScheduleDatabase* db = StationScheduleDatabase::load_txt(input_stream);
    GlobalTrainDatabase::print();
    db->print();
    input_stream.close();


    std::cout << "============ Save and load database in binary mode (database.bin): ============" << std::endl;

    output_stream.open("database.bin", std::ios_base::binary);
    station1.save_bin(output_stream);
    output_stream.close();

    GlobalTrainDatabase::clear();
    input_stream.open("database.bin", std::ios_base::binary);
    db = StationScheduleDatabase::load_bin(input_stream);
    GlobalTrainDatabase::print();
    db->print();
    input_stream.close();
}
