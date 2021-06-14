#ifndef TRAINDATABASE_H
#define TRAINDATABASE_H

#include <string>
#include <ctime>
#include <vector>
#include <functional>
#include <fstream>
#include <map>

#include <boost/optional.hpp>

#include "Sort.h"

enum struct TrainType
{
    Standard,
    International,
    Suburban,
    HighSpeed
};

const int TRAIN_TYPE_COUNT = 4;
const std::string TRAIN_TYPE_NAMES[TRAIN_TYPE_COUNT] = {"Standard", "International", "Suburban", "HighSpeed"};

std::ostream& operator<<(std::ostream& os, TrainType type);

struct Train
{
    int16_t id; //use specific bit sizes to make binary database files (hopefully) portable
    TrainType type;
    std::string name;

    Train(short id, TrainType type = TrainType::Standard, std::string name = "");

    void save_txt(std::ofstream& file) const;
    void save_bin(std::ofstream& file) const;

    static Train load_txt(std::ifstream& file);
    static Train load_bin(std::ifstream& file);
};

std::ostream& operator<<(std::ostream& os, const Train& train);

struct TicketInfo
{
    int32_t ticket_id;
    int16_t train_id;
    time_t arrival_time;
    time_t departure_time;
    int16_t tickets_wanted;
    int16_t tickets_available;

    TicketInfo(int32_t ticket_id, int16_t train_id, time_t arrival_time, time_t departure_time, int16_t tickets_wanted, int16_t tickets_available);
    TicketInfo() = default;

    float get_popularity_ratio() const;

    void save_txt(std::ofstream& file) const;
    void save_bin(std::ofstream& file) const;

    static TicketInfo load_txt(std::ifstream& file);
    static TicketInfo load_bin(std::ifstream& file);


    static int compare_ticket_ids(const TicketInfo& ticket1, const TicketInfo& ticket2);
    static int compare_train_ids(const TicketInfo& ticket1, const TicketInfo& ticket2);
    static int compare_train_types(const TicketInfo& ticket1, const TicketInfo& ticket2);
    static int compare_popularity(const TicketInfo& ticket1, const TicketInfo& ticket2);


    friend std::ostream& operator<<(std::ostream& os, const TicketInfo& ticket);
    std::string to_string(bool localizeTime = true) const;

private:
    std::ostream& output_as_text(std::ostream& os, bool humanReadable = true) const;
};



class StationScheduleDatabase
{
public:
    ~StationScheduleDatabase();

    int station_id;

    void add_ticket(TicketInfo ticket_info);
    boost::optional<TicketInfo> get_ticket(int id) const;
    bool delete_ticket(int id); //returns true if ticket with such id existed before (and has been removed)
    void clear();


    //Returns IDs as it's more efficient than copying Train objects
    std::vector<int32_t> find_all_tickets(const std::function<bool(const TicketInfo&)>& predicate) const;
    //Returns const reference (might get invalidated if the original tickets map is modified!)
    const std::map<int32_t, TicketInfo>& get_tickets_map() const;
    TicketInfo* get_tickets_array() const;
    int get_ticket_count() const;
    TicketInfo* get_tickets_sorted(std::vector<sort::ComparatorFunction<TicketInfo>> comparators) const;

    void print() const;

    void save_txt(std::ofstream& file) const;
    void save_bin(std::ofstream& file) const;

    static StationScheduleDatabase* load_txt(std::ifstream& file);
    static StationScheduleDatabase* load_bin(std::ifstream& file);


private:
    std::map<int32_t, TicketInfo> tickets;
};



//std::ostream& operator<<(std::ostream& os, const TicketInfo& ticket);

class GlobalTrainDatabase
{
public:
    static const int MAX_TRAINS = 10000;

    static void add_train(Train train);
    static bool delete_train(int id);
    static boost::optional<Train> get_train(int id);
    static void clear();
    static void print();

private:
    static boost::optional<Train> trains_by_id[MAX_TRAINS];
};


#endif // TRAINDATABASE_H
