#include "Train.hpp"
#include <string>
#include <map>

// 定义静态成员
BPT<int, 500> Train::trains;                   // trainID -> int
BPT<queue, 500> Train::train_queue;            // trainID -> queue
BPT<train_ticket, 500> Train::train_ticket;    // stations -> train_ticket
MemoryRiver<Block_train> Train::train_river;   // int -> Block_train
BPT<ticket, 500> Train::train_ticket;          // trainID -> 起始位置
MemoryRiver<ticket> Train::train_ticket_river; // 时间顺序排列每日票数
std::map<std::string, int> Train::train_list;  // release

void Train::add_train(const char *trainID, const char *stations, const char *prices, const char *startTime, const char *travelTime, const char *stopoverTime, const char *saleDate, char type)
{
    // std::cout << trainID << " " << stations << " " << prices << " " << startTime << " " << travelTime << " " << stopoverTime << " " << saleDate << " " << type << std::endl;
    if (trains.Find1(trainID))
    {
        std::cout << "-1" << std::endl;
        return;
    }
    int train_num;
    train_river.get_info(train_num, 1);
    train_num++;
    Block_train new_train(trainID, stations, prices, startTime, travelTime, stopoverTime, saleDate, type, train_num);
    train_river.write_info(train_num, 1);
    train_river.write(new_train, train_num);
    trains.insert(trainID, train_num);
    train_list[trainID] = train_num;
    std::cout << "0" << std::endl;
    return;
}

void Train::release_train(const char *trainID)
{
    // std::cout << trainID << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) != train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    train_list[trainID] = pos;
    std::cout << "0" << std::endl;
    return;
}

void Train::query_train(const char *trainID)
{
    // std::cout << trainID << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    std::cout << train.trainID << " " << train.stationNum << " " << train.seatNum << " " << train.stations << " " << train.prices << " " << train.startTime << " " << train.travelTime << " " << train.stopoverTime << " " << train.saleDate << " " << train.type << std::endl;
    return;
}

void Train::delete_train(const char *trainID)
{
    // std::cout << trainID << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    trains.erase(trainID);
    train_list.erase(trainID);
    train_river.erase(pos);
    std::cout << "0" << std::endl;
    return;
}

void Train::modify_train(const char *trainID, const char *stations, const char *prices, const char *startTime, const char *travelTime, const char *stopoverTime, const char *saleDate, char type)
{
    // std::cout << trainID << " " << stations << " " << prices << " " << startTime << " " << travelTime << " " << stopoverTime << " " << saleDate << " " << type << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    train.modify_train(stations, prices, startTime, travelTime, stopoverTime, saleDate, type);
    train_river.write(train, pos);
    std::cout << "0" << std::endl;
    return;
}

void Train::query_ticket(const char *trainID, const char *date, const char *from, const char *to, int flag)
{
    // std::cout << trainID << " " << date << " " << from << " " << to << " " << flag << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    if (flag == 0)
    {
        train.query_ticket(date, from, to);
    }
    else
    {
        train.query_transfer(date, from, to);
    }
    return;
}

void Train::buy_ticket(const char *trainID, const char *date, const char *from, const char *to, int num, int *ticketNum, int *ticketPrice)
{
    // std::cout << trainID << " " << date << " " << from << " " << to << " " << num << " " << ticketNum << " " << ticketPrice << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    train.buy_ticket(date, from, to, num, ticketNum, ticketPrice);
    return;
}

void Train::refund_ticket(const char *trainID, const char *date, const char *from, const char *to, int num, int *ticketNum, int *ticketPrice)
{
    // std::cout << trainID << " " << date << " " << from << " " << to << " " << num << " " << ticketNum << " " << ticketPrice << std::endl;
    int pos = trains.Find2(trainID);
    if (pos == -1 || train_list.find(trainID) == train_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_train train;
    train_river.read(train, pos);
    train.refund_ticket(date, from, to, num, ticketNum, ticketPrice);
    return;
}

void Train::query_transfer(const char *from, const char *to, int type)
{
    // std::cout << from << " " << to << " " << date << std::endl;
    for (auto it = train_list.begin(); it != train_list.end(); ++it)
    {
        Block_train train;
        train_river.read(train, it->second);
        train.query_transfer(date, from, to);
    }
    return;
}

