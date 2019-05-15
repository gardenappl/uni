#include <functional>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include "TrainDatabase.h"


std::ostream& operator<<(std::ostream& os, TrainType type)
{
    if((char)type < TRAIN_TYPE_COUNT)
        os << TRAIN_TYPE_NAMES[(size_t)type];
    else
        os.setstate(std::ios_base::failbit);
    return os;
}




boost::optional<Train> GlobalTrainDatabase::trains_by_id[GlobalTrainDatabase::MAX_TRAINS] = {boost::optional<Train>()};

void GlobalTrainDatabase::add_train(Train train)
{
    GlobalTrainDatabase::trains_by_id[train.id] = train;
}

boost::optional<Train> GlobalTrainDatabase::get_train(int id)
{
    return GlobalTrainDatabase::trains_by_id[id];
}

bool GlobalTrainDatabase::delete_train(int id)
{
    if(GlobalTrainDatabase::trains_by_id[id])
    {
        GlobalTrainDatabase::trains_by_id[id] = boost::optional<Train>();
        return true;
    }
    return false;
}

void GlobalTrainDatabase::clear()
{
    for(int i = 0; i < MAX_TRAINS; i++)
    {
        GlobalTrainDatabase::trains_by_id[i] = boost::optional<Train>();
    }
//    GlobalTrainDatabase::trains_by_id[GlobalTrainDatabase::MAX_TRAINS] = {boost::optional<Train>()};
}

void GlobalTrainDatabase::print()
{
    for(int i = 0; i < GlobalTrainDatabase::MAX_TRAINS; i++)
    {
        if(GlobalTrainDatabase::trains_by_id[i])
        {
            std::cout << *GlobalTrainDatabase::trains_by_id[i] << std::endl;
        }
    }
}



Train::Train(short id, TrainType type, std::string name)
: id(id), type(type), name(name)
{}

std::ostream& operator<<(std::ostream& os, const Train& train)
{
    os << "Train #" << train.id;
    if(train.name != "")
        os << " \"" << train.name << "\"";
    os << std::endl;
    os << "\tType: " << train.type;

    return os;
}

void Train::save_txt(std::ofstream& file) const
{
    if(file.is_open())
        file << *this;
    else
        throw std::invalid_argument("File not open!");
}

Train Train::load_txt(std::ifstream& file)
{
    std::string temp_string;
    if(file.is_open())
    {
        file >> temp_string;
        if(temp_string != "Train")
            throw std::invalid_argument("Bad file! (expected 'Train')");

        //Read ID
        file.ignore(2); //ignore whitespace and #
        int id;
        file >> id;

        //Read name
        std::string name;
        char temp_char = file.get();
        if(temp_char != '\n')
        {
            file.ignore(); //ignore first quotation mark
            std::getline(file, name);
            name.pop_back(); //remove last quotation mark
            file.ignore(); //read end-of-line
        }
        else
            name = "";

        //Read type
        file >> temp_string; //ignore "Type: "

        char type;
        file >> temp_string;
        for(int i = 0; i < TRAIN_TYPE_COUNT; i++)
        {
            if(TRAIN_TYPE_NAMES[i] == temp_string)
                type = (char)i;
        }

        return Train(id, (TrainType)type, name);
    }
    else
        throw std::invalid_argument("File not open!");
}

void Train::save_bin(std::ofstream& file) const
{
    if(file.is_open())
    {
        file.write(reinterpret_cast<const char*>(&this->id), sizeof(int16_t));
        uint32_t length = (uint32_t)this->name.length();
        file.write(reinterpret_cast<const char*>(&length), sizeof(uint32_t));
        file.write(this->name.c_str(), this->name.length());
        file.put((char)this->type);
    }
    else
        throw std::invalid_argument("File not open!");
}

Train Train::load_bin(std::ifstream& file)
{
    if(file.is_open())
    {
        int16_t id;
        file.read(reinterpret_cast<char*>(&id), sizeof(int16_t));

        uint32_t string_size;
        file.read(reinterpret_cast<char*>(&string_size), sizeof(uint32_t));
        char* string_buffer = new char[string_size];
        file.read(string_buffer, string_size);
        std::string name(string_buffer, string_size);
        delete string_buffer;

        char type = file.get();
        return Train(id, (TrainType)type, name);
    }
    else
        throw std::invalid_argument("File not open!");
}




TicketInfo::TicketInfo(int32_t ticket_id, int16_t train_id, time_t arrival_time, time_t departure_time,
        int16_t tickets_wanted, int16_t tickets_available)
: ticket_id(ticket_id), train_id(train_id), arrival_time(arrival_time), departure_time(departure_time),
        tickets_wanted(tickets_wanted), tickets_available(tickets_available)
{}

float TicketInfo::get_popularity_ratio() const
{
    return (float)this->tickets_wanted / this->tickets_available;
}

std::ostream& operator<<(std::ostream& os, const TicketInfo& ticket)
{
    ticket.output_as_text(os, true);
//    os << ticket.get_popularity_ratio();
    return os;
}

std::string TicketInfo::to_string(bool humanReadable) const
{
    std::stringstream ss;
    this->output_as_text(ss, humanReadable);
    return ss.str();
}

std::ostream& TicketInfo::output_as_text(std::ostream& os, bool humanReadable) const
{
    os << "Ticket #" << this->ticket_id << std::endl;
    if(humanReadable)
    {
        os << *GlobalTrainDatabase::get_train(this->train_id) << std::endl;
    }
    else
    {
        os << "Train #" << this->train_id << std::endl;
    }
    if(humanReadable)
    {
        os << "Arrival time: " << ctime(&this->arrival_time);
        os << "Departure time: " << ctime(&this->departure_time);
    }
    else
    {
        os << "Arrival time: " << this->arrival_time << std::endl;
        os << "Departure time: " << this->departure_time << std::endl;
    }
    os << "Tickets wanted: " << this->tickets_wanted << std::endl;
    os << "Tickets available: " << this->tickets_available << std::endl;
    if(humanReadable)
    {
        os << "(Popularity ratio: " << this->get_popularity_ratio() << ")";
    }
    return os;
}

void TicketInfo::save_txt(std::ofstream& file) const
{
    if(file.is_open())
        this->output_as_text(file, false);
    else
        throw std::invalid_argument("File not open!");
}

TicketInfo TicketInfo::load_txt(std::ifstream& file)
{
    std::string temp_string;
    if(file.is_open())
    {
        //Read ticket ID
        file >> temp_string;
        if(temp_string != "Ticket")
            throw std::invalid_argument("Bad file! (expected 'Ticket')");
        file.ignore(2); //ignore whitespace and #

        int ticket_id;
        file >> ticket_id;


        //Read train ID
        file >> temp_string;
        file.ignore(2); //ignore whitespace and #

        int train_id;
        file >> train_id;

        file.ignore(1000, ':'); //ignore "Arrival time:"
        time_t arrival_time;
        file >> arrival_time;

        file.ignore(1000, ':'); //ignore "Departure time:"
        time_t departure_time;
        file >> departure_time;

        file.ignore(1000, ':'); //ignore "Tickets wanted:"
        int tickets_wanted;
        file >> tickets_wanted;

        file.ignore(1000, ':'); //ignore "Tickets available:"
        int tickets_available;
        file >> tickets_available;

        return TicketInfo(ticket_id, train_id, arrival_time, departure_time, tickets_wanted, tickets_available);
    }
    else
        throw std::invalid_argument("File not open!");
}

void TicketInfo::save_bin(std::ofstream& file) const
{
    if(file.is_open())
    {
        file.write(reinterpret_cast<const char*>(&this->ticket_id), sizeof(int32_t));
        file.write(reinterpret_cast<const char*>(&this->train_id), sizeof(int16_t));
        int64_t temp_int = (int64_t)this->arrival_time;
        file.write(reinterpret_cast<const char*>(&temp_int), sizeof(int64_t));
        temp_int = (int64_t)this->departure_time;
        file.write(reinterpret_cast<const char*>(&temp_int), sizeof(int64_t));
        file.write(reinterpret_cast<const char*>(&this->tickets_wanted), sizeof(int16_t));
        file.write(reinterpret_cast<const char*>(&this->tickets_available), sizeof(int16_t));
    }
    else
        throw std::invalid_argument("File not open!");
}

TicketInfo TicketInfo::load_bin(std::ifstream& file)
{
    if(file.is_open())
    {
        int32_t ticket_id;
        file.read(reinterpret_cast<char*>(&ticket_id), sizeof(int32_t));
        int16_t train_id;
        file.read(reinterpret_cast<char*>(&train_id), sizeof(int16_t));
        int64_t arrival_time;
        file.read(reinterpret_cast<char*>(&arrival_time), sizeof(int64_t));
        int64_t departure_time;
        file.read(reinterpret_cast<char*>(&departure_time), sizeof(int64_t));
        int16_t tickets_wanted;
        file.read(reinterpret_cast<char*>(&tickets_wanted), sizeof(int16_t));
        int16_t tickets_available;
        file.read(reinterpret_cast<char*>(&tickets_available), sizeof(int16_t));

        return TicketInfo(ticket_id, train_id, arrival_time, departure_time, tickets_wanted, tickets_available);
    }
    else
        throw std::invalid_argument("File not open!");
}

int TicketInfo::compare_popularity(const TicketInfo& ticket1, const TicketInfo& ticket2)
{
    float popularity1 = ticket1.get_popularity_ratio();
    float popularity2 = ticket2.get_popularity_ratio();
    if(popularity1 < popularity2)
        return -1;
    else
        return popularity1 != popularity2;
}

int TicketInfo::compare_ticket_ids(const TicketInfo& ticket1, const TicketInfo& ticket2)
{
    if(ticket1.ticket_id < ticket2.ticket_id)
        return -1;
    else
        return ticket1.ticket_id != ticket2.ticket_id;
}

int TicketInfo::compare_train_ids(const TicketInfo& ticket1, const TicketInfo& ticket2)
{
    if(ticket1.train_id < ticket2.train_id)
        return -1;
    else
        return ticket1.train_id != ticket2.train_id;
}

int TicketInfo::compare_train_types(const TicketInfo& ticket1, const TicketInfo& ticket2)
{
    boost::optional<Train> train1 = GlobalTrainDatabase::get_train(ticket1.train_id);
    boost::optional<Train> train2 = GlobalTrainDatabase::get_train(ticket2.train_id);
    if(train1->type < train2->type)
        return -1;
    else
        return train1->type != train2->type;
}


void StationScheduleDatabase::add_ticket(TicketInfo ticket_info)
{
//    this->tickets.insert(std::pair<int32_t, TicketInfo>(ticket_info.ticket_id, ticket_info));
    this->tickets[ticket_info.ticket_id] = ticket_info;
}

boost::optional<TicketInfo> StationScheduleDatabase::get_ticket(int id)
{
    try
    {
        return boost::optional<TicketInfo>(this->tickets.at(id));
    }
    catch(const std::out_of_range& e)
    {
        return boost::none;
    }
}

bool StationScheduleDatabase::delete_ticket(int id)
{
    if(this->tickets.find(id) != this->tickets.end())
    {
        this->tickets.erase(id);
        return true;
    }
    return false;
}

std::vector<int> StationScheduleDatabase::find_all_tickets(const std::function<bool(const TicketInfo&)>& predicate) const
{
    std::vector<int> tickets;
    for(const std::pair<int, TicketInfo>& element : this->tickets)
    {
        if(predicate(element.second))
        {
            tickets.push_back(element.first);
        }
    }
    return tickets;
}

const std::map<int32_t, TicketInfo>& StationScheduleDatabase::get_tickets_map()
{
    return this->tickets;
}

TicketInfo* StationScheduleDatabase::get_tickets_array()
{
    TicketInfo* array = new TicketInfo[this->tickets.size()];

    int i = 0;
    for(auto kvp : this->tickets)
    {
        array[i] = kvp.second;
        i++;
    }
    return array;
}

int StationScheduleDatabase::get_ticket_count()
{
    return this->tickets.size();
}

void StationScheduleDatabase::print() const
{
    for(const std::pair<int, TicketInfo>& element : this->tickets)
    {
        std::cout << element.second << std::endl << std::endl;
    }
}

void StationScheduleDatabase::save_txt(std::ofstream& file) const
{
    if(file.is_open())
    {
        for(int i = 0; i < GlobalTrainDatabase::MAX_TRAINS; i++)
        {
            boost::optional<Train> train = GlobalTrainDatabase::get_train(i);
            if(train)
            {
                train->save_txt(file);
                file << std::endl;
            }
        }
        file << "Stations: " << std::endl;
        for(const std::pair<int, TicketInfo>& element : this->tickets)
        {
            element.second.save_txt(file);
        }
    }
    else
    {
        throw std::invalid_argument("File not open!");
    }
}



StationScheduleDatabase* StationScheduleDatabase::load_txt(std::ifstream& file)
{
    if(file.is_open())
    {
        //Part 1: read trains
        while(file.peek() != EOF)
        {
            if(file.peek() != 'S') //while we're not reading Stations
            {
                Train train = Train::load_txt(file);
                GlobalTrainDatabase::add_train(train);
                file.get(); //return end-of-line
            }
            else
            {
                break;
            }
        }
        file.ignore(10000, '\n'); //skip to next line
        StationScheduleDatabase* db = new StationScheduleDatabase;

        //Part 2: read stations
        while(file.peek() == 'T')
        {
            db->add_ticket(TicketInfo::load_txt(file));
            file.get();
        }
        return db;
    }
    else
        throw std::invalid_argument("File not open!");
}

void StationScheduleDatabase::save_bin(std::ofstream& file) const
{
    if(file.is_open())
    {
        for(int i = 0; i < GlobalTrainDatabase::MAX_TRAINS; i++)
        {
            boost::optional<Train> train = GlobalTrainDatabase::get_train(i);
            if(train)
            {
                file.put('t'); //t means a train is stored after it
                train->save_bin(file);
            }

        }
        for(const std::pair<int, TicketInfo>& element : this->tickets)
        {
            file.put('s'); //s means a ticket is stored
            element.second.save_bin(file);
        }
    }
    else
    {
        throw std::invalid_argument("File not open!");
    }
}


StationScheduleDatabase* StationScheduleDatabase::load_bin(std::ifstream& file)
{
    if(file.is_open())
    {
        StationScheduleDatabase* db = new StationScheduleDatabase;

        while(file.peek() != EOF)
        {
            if(file.peek() == 't')
            {
                file.ignore();
                GlobalTrainDatabase::add_train(Train::load_bin(file));
            }
            else if(file.peek() == 's')
            {
                file.ignore();
                db->add_ticket(TicketInfo::load_bin(file));
            }
            else
                throw std::invalid_argument("File corrupted or is in unknown format.");
        }
        return db;
    }
    else
    {
        throw std::invalid_argument("File not open!");
    }
}

void StationScheduleDatabase::clear()
{
    this->tickets.clear();
}

StationScheduleDatabase::~StationScheduleDatabase()
{
    this->tickets.clear();
}
