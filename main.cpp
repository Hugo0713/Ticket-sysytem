#include <iostream>
#include <string>
#include <cstring>
#include "Utils/Tokenscanner.hpp"
#include "Utils/Time.hpp"
#include "src/User.hpp"
#include "src/Train.hpp"


void processLine(const std::string &line)
{
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    std::string command = scanner.nextToken();
    if (command == "register")
    {
        std::string username = scanner.nextToken();
        std::string password = scanner.nextToken();
        std::string name = scanner.nextToken();
        std::string mailAddr = scanner.nextToken();
        int privilege = std::stoi(scanner.nextToken());
        User::registerUser(username, password, name, mailAddr, privilege);
    }
    else if (command == "login")
    {
        std::string username = scanner.nextToken();
        std::string password = scanner.nextToken();
        User::login(username, password);
    }
    else if (command == "logout")
    {
        User::logout();
    }
    else if (command == "query_profile")
    {
        User::queryProfile();
    }
    else if (command == "modify_profile")
    {
        std::string name = scanner.nextToken();
        std::string mailAddr = scanner.nextToken();
        User::modifyProfile(name, mailAddr);
    }
    else if (command == "modify_privilege")
    {
        std::string username = scanner.nextToken();
        int privilege = std::stoi(scanner.nextToken());
        User::modifyPrivilege(username, privilege);
    }
    else if (command == "add_train")
    {
        std::string trainID = scanner.nextToken();
        int stationNum = std::stoi(scanner.nextToken());
        std::string stations[100];
        for (int i = 0; i < stationNum; ++i)
        {
            stations[i] = scanner.nextToken();
        }
        int seatNum = std::stoi(scanner.nextToken());
        int prices[100];
        for (int i = 0; i < seatNum; ++i)
        {
            prices[i] = std::stoi(scanner.nextToken());
        }
        Time startTime(scanner.nextToken());
        int travelTime[100];
        for (int i = 0; i < stationNum; ++i)
        {
            travelTime[i] = std::stoi(scanner.nextToken());
        }
        int stopoverTime[100];
        for (int i = 0; i < stationNum; ++i)
        {
            stopoverTime[i] = std::stoi(scanner.nextToken());
        }
        int saleDate[2];
        saleDate[0] = std::stoi(scanner.nextToken());
        saleDate[1] = std::stoi(scanner.nextToken());
        char type = scanner.nextToken()[0];
        int stationPos = std::stoi(scanner.nextToken());
        Train::addTrain(trainID, stationNum, stations, seatNum, prices, startTime, travelTime, stopoverTime, saleDate, type, stationPos);
    }
    else if (command == "release_train")
    {
        std::string trainID = scanner.nextToken();
        Train::releaseTrain(trainID);
    }
    else if (command == "query_train")
    {
        std::string trainID = scanner.nextToken();
        Train::queryTrain(trainID);
    }
    else if (command == "delete_train")
    {
        std::string trainID = scanner.nextToken();
        Train::deleteTrain(trainID);
    }
    else if (command == "query_ticket")
    {
        std::string date = scanner.nextToken();
        std::string from = scanner.nextToken();
        std::string to = scanner.nextToken();
        char type = scanner.nextToken()[0];
        Train::queryTicket(date, from, to, type);
    }
    else if (command == "query_transfer")
    {
        std::string date = scanner.nextToken();
        std::string from = scanner.nextToken();
        std::string to = scanner.nextToken();
        Train::queryTransfer(date, from, to);
    }
    else if (command == "buy_ticket")
    {
        std::string date = scanner.nextToken();
        std::string trainID = scanner.nextToken();
        char from[20];
        strcpy(from, scanner.nextToken().c_str());
        char to[20];
        strcpy(to, scanner.nextToken().c_str());
        char ticketKind = scanner.nextToken()[0];
        int ticketNum = std::stoi(scanner.nextToken());
        Train::buyTicket(date, trainID, from, to, ticketKind, ticketNum);
    }
    else if (command == "query_order")
    {
        std::string date = scanner.nextToken();
        Train::queryOrder(date);
    }
    else if (command == "refund_ticket")
    {
        std::string date = scanner.nextToken();
        int ticketNum = std::stoi(scanner.nextToken());
        Train::refundTicket(date, ticketNum);
    }
    else if (command == "clean")
    {
        User::clean();
        Train::clean();
    }
    else
    {
        throw std::invalid_argument("Invalid command\n");
    }
}



int main()
{
    while (true)
    {
        try
        {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
            {
                return 0;
            }
            processLine(input);
        }
        catch (const std::exception &e)
        {
            std::cout << e.what();
        }
    }
    return 0;
}