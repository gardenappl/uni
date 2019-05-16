#include "BenchmarkMode.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <boost/optional.hpp>

#include "TrainDatabase.h"
#include "Sort.h"

void BenchmarkMode::run()
{
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> train_id_distribution(0, GlobalTrainDatabase::MAX_TRAINS - 1); // guaranteed unbiased
    std::uniform_int_distribution<char> train_type_distribution(0, TRAIN_TYPE_COUNT - 1);
    std::uniform_int_distribution<int> bool_distribution(0, 1);
    std::uniform_int_distribution<time_t> time_distribution(0, 1000000);
    std::uniform_int_distribution<int> tickets_distribution(1, 100);


    StationScheduleDatabase* db = new StationScheduleDatabase;
    GlobalTrainDatabase::clear();



    for(int i = 0; i < 1000000; i++)
    {
        int train_id = train_id_distribution(rng);
        if(!GlobalTrainDatabase::get_train(train_id))
        {
            GlobalTrainDatabase::add_train(Train(train_id, (TrainType)train_type_distribution(rng), bool_distribution(rng) ? "" : "A Train named " + std::to_string(train_id)));
        }
        db->add_ticket(TicketInfo((int32_t)i, (int16_t)train_id, time_distribution(rng), time_distribution(rng),
                (int16_t)tickets_distribution(rng), (int16_t)tickets_distribution(rng)));
    }

    std::cout << "Sort by train type (counting sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_train_types});
                delete array;
            });

    std::cout << "Sort by train ID (radix sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_train_ids});
                delete array;
            });

    std::cout << "Sort by popularity (optimized merge sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_popularity});
                delete array;
            });

    std::cout << "Sort by popularity and train type (optimized merge sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_popularity, TicketInfo::compare_train_types});
                delete array;
            });

    std::cout << "Sort by train type and popularity (optimized merge sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_train_types, TicketInfo::compare_popularity});
                delete array;
            });

    std::cout << "Sort by train type, popularity and ticket ID (optimized merge sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_train_types, TicketInfo::compare_popularity, TicketInfo::compare_ticket_ids});
                delete array;
            });

    std::cout << "Sort by train type, popularity, ticket ID and train ID (optimized merge sort)... ";
    BenchmarkMode::test_scenario([db]()
            {
                TicketInfo* array = db->get_tickets_sorted({TicketInfo::compare_train_types, TicketInfo::compare_popularity, TicketInfo::compare_ticket_ids, TicketInfo::compare_train_ids});
                delete array;
            });
}

void BenchmarkMode::test_scenario(std::function<void()> benchmark_action)
{
    auto start = std::chrono::system_clock::now();

    benchmark_action();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << diff.count() << " s\n";
}
