#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <string>
#include <map>
#include "../Database/BPT.hpp"
#include "../Database/MemoryRiver.hpp"

struct train_ticket // 用于query_ticket和query_transfer
{
    char trainID[20] = {'\0'};
    char date[8] = {'\0'}; // 从该站出发的日期与时间
    int price;             // 到下一站的价格
};

struct ticket // 按照时间顺序排列
{
    int seat[100];  // 各站之间的票数
    int price[100]; // 各站之间的价格
};

struct queue
{
    char username[20] = {'\0'};
    char from[40] = {'\0'};
    char to[40] = {'\0'};
    char date[4] = {'\0'};
    int num;
    int price;
};

class Block_train
{
private:
    char trainID[20] = {'\0'};
    int stationNum = 0;
    char stations[100][40] = {{'\0'}};
    int seatNum = 0;
    int prices[100] = {};
    char startTime[4] = {'\0'};
    int travelTime[100];
    int stopoverTime[100];
    char saleDate[8];
    char type;
    int stationPos;

public:
    Block_train() = default;
    Block_train(const Block_train &other)
    {
        strcpy(this->trainID, other.trainID);
        this->stationNum = other.stationNum;
        for (int i = 0; i < stationNum; ++i)
        {
            strcpy(this->stations[i], other.stations[i]);
        }
        this->seatNum = other.seatNum;
        for (int i = 0; i < seatNum; ++i)
        {
            this->prices[i] = other.prices[i];
        }
        strcpy(this->startTime, other.startTime);
        for (int i = 0; i < stationNum; ++i)
        {
            this->travelTime[i] = other.travelTime[i];
        }
        for (int i = 0; i < stationNum; ++i)
        {
            this->stopoverTime[i] = other.stopoverTime[i];
        }
        strcpy(this->saleDate, other.saleDate);
        this->type = other.type;
        this->stationPos = other.stationPos;
    }
    Block_train &operator=(const Block_train &other)
    {
        if (this == &other)
        {
            return *this;
        }
        strcpy(this->trainID, other.trainID);
        this->stationNum = other.stationNum;
        for (int i = 0; i < stationNum; ++i)
        {
            strcpy(this->stations[i], other.stations[i]);
        }
        this->seatNum = other.seatNum;
        for (int i = 0; i < seatNum; ++i)
        {
            this->prices[i] = other.prices[i];
        }
        strcpy(this->startTime, other.startTime);
        for (int i = 0; i < stationNum; ++i)
        {
            this->travelTime[i] = other.travelTime[i];
        }
        for (int i = 0; i < stationNum; ++i)
        {
            this->stopoverTime[i] = other.stopoverTime[i];
        }
        strcpy(this->saleDate, other.saleDate);
        this->type = other.type;
        this->stationPos = other.stationPos;
        return *this;
    }
    bool operator==(const Block_train &other)
    {
        return strcmp(this->trainID, other.trainID) == 0;
    }
    bool operator!=(const Block_train &other)
    {
        return strcmp(this->trainID, other.trainID) != 0;
    }
    bool operator<(const Block_train &other)
    {
        return strcmp(this->trainID, other.trainID) < 0;
    }
    bool operator>(const Block_train &other)
    {
        return strcmp(this->trainID, other.trainID) > 0;
    }
};

class Train
{
private:
    static BPT<int, 500> trains;                   // trainID -> int
    static BPT<queue, 500> train_queue;            // trainID -> queue
    static BPT<train_ticket, 500> train_ticket;    // stations -> train_ticket
    static MemoryRiver<Block_train> train_river;   // int -> Block_train
    static BPT<ticket, 500> train_ticket;          // trainID -> 起始位置
    static MemoryRiver<ticket> train_ticket_river; // 时间顺序排列每日票数
    static std::map<std::string, int> train_list;  // release

public:
    Train()
    {
        trains.initialise("trains");
        train_queue.initialise("train_queue");
        train_ticket.initialise("train_ticket");
        train_river.initialise("train_river");
        train_ticket.initialise("train_ticket");
        train_ticket_river.initialise("train_ticket_river");
    }

    void add_train(const char *trainID, int stationNum, int seatNum, char stations[][40], int *prices, const char *startTime, int *travelTime, int *stopoverTime, const char *saleDate, char type);
    void delete_train(const char *trainID);
    void release_train(const char *trainID);
    void query_train(const char *trainID, const char *startTime);
    void query_ticket(const char *trainID, const char *from, const char *to, int type);
    void query_transfer(const char *from, const char *to, int type);
};

#endif