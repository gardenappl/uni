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

void BenchmarkMode::run(std::function<void(StationScheduleDatabase*&, std::string)> benchmark_action, std::string file_name)
{
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = end - start;

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> train_id_distribution(0, GlobalTrainDatabase::MAX_TRAINS - 1); // guaranteed unbiased
    std::uniform_int_distribution<char> train_type_distribution(0, TRAIN_TYPE_COUNT - 1);
    std::uniform_int_distribution<int> bool_distribution(0, 1);
    std::uniform_int_distribution<time_t> time_distribution(0, 1000000);
    std::uniform_int_distribution<int> tickets_distribution(1, 100);

    int n = 1;
    do
    {
        BenchmarkMode::adjust_n(diff, &n);
        StationScheduleDatabase* db = new StationScheduleDatabase;
        GlobalTrainDatabase::clear();

        auto start = std::chrono::system_clock::now();


        for(int i = 0; i < n; i++)
        {
            int train_id = train_id_distribution(rng);
            if(!GlobalTrainDatabase::get_train(train_id))
            {
                GlobalTrainDatabase::add_train(Train(train_id, (TrainType)train_type_distribution(rng), bool_distribution(rng) ? "" : "A Train named " + std::to_string(train_id)));
            }
            db->add_ticket(TicketInfo((int32_t)i, (int16_t)train_id, time_distribution(rng), time_distribution(rng),
                    (int16_t)tickets_distribution(rng), (int16_t)tickets_distribution(rng)));
        }

        benchmark_action(db, file_name);

        auto end = std::chrono::system_clock::now();
        diff = end - start;

        delete db;
        std::cout << diff.count() << " s File size: " <<
                std::to_string(BenchmarkMode::get_file_size(file_name)) << " bytes" << std::endl;
    }while(diff < std::chrono::seconds(10));

    std::cout << "Average per 10 seconds: " << (int)(n * std::chrono::seconds(10) / diff) << std::endl;

    std::cin.ignore(); //stop to check RAM usage at the end of the benchmark
}

int BenchmarkMode::n_40_percent = 0;

void BenchmarkMode::adjust_n(std::chrono::duration<double> last_duration, int* n)
{
    if(last_duration < std::chrono::seconds(4))
    {
        *n *= 2;
    }
    else if(last_duration < std::chrono::seconds(8))
    {
        if(!BenchmarkMode::n_40_percent)
            BenchmarkMode::n_40_percent = *n;
        *n += BenchmarkMode::n_40_percent / 4;
    }
    else
    {
        *n += BenchmarkMode::n_40_percent / 8;
    }
}

void BenchmarkMode::benchmark_action_binary_test(StationScheduleDatabase*& database, std::string file_name)
{
    std::ofstream output_file(file_name, std::ios_base::binary);
    database->save_bin(output_file);
    output_file.close();
    delete database;

    std::ifstream input_file(file_name, std::ios_base::binary);
    database=StationScheduleDatabase::load_bin(input_file);
    input_file.close();

    BenchmarkMode::test_search(database);
}

void BenchmarkMode::benchmark_action_txt_test(StationScheduleDatabase*& database, std::string file_name)
{
    std::ofstream output_file(file_name);
    database->save_txt(output_file);
    output_file.close();
    delete database;

    std::ifstream input_file(file_name);
    database=StationScheduleDatabase::load_txt(input_file);
    input_file.close();

    BenchmarkMode::test_search(database);
}

void BenchmarkMode::test_search(StationScheduleDatabase* database)
{
    const float pop_ratio = 0.5f;
    const auto search_pop_ratio = [pop_ratio](const TicketInfo& ticket)
            {
                return ticket.get_popularity_ratio() >= 0.5f;
            };


    const std::string query_string = "named";
    const auto search_query_string = [query_string](const TicketInfo& ticket)
            {
                boost::optional<Train> train = GlobalTrainDatabase::get_train(ticket.train_id);
                return train && train->name.find(query_string) != std::string::npos;
            };


    time_t arrival_time = 500000;
    const auto search_arrival_time = [arrival_time](const TicketInfo& ticket)
            {
                return ticket.arrival_time <= arrival_time;
            };

    std::cout << database->get_tickets_map().size() << " entries total; ";
    std::vector<int> search_result = database->find_all_tickets(search_pop_ratio);
    std::cout << search_result.size() << " entries found, ";
    search_result.clear();
    search_result = database->find_all_tickets(search_query_string);
    std::cout << search_result.size() << " entries found, ";
    search_result.clear();
    search_result = database->find_all_tickets(search_arrival_time);
    std::cout << search_result.size() << " entries found." << std::endl;
    search_result.clear();
}

//copied from SO
int BenchmarkMode::get_file_size(std::string file_name)
{
    std::ifstream in(file_name, std::ifstream::ate | std::ifstream::binary); //open file at the end
    int file_size = (int)in.tellg(); //get the crrent position in bytes
    in.close();
    return file_size;
}
