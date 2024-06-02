#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include "Database/BPT.hpp"
#include "Database/MemoryRiver.hpp"
#include "Utils/Tokenscanner.hpp"
#include "src/User.hpp"
//#include "src/Train.hpp"

void processLine(const std::string &line)
{
    User user;
    TokenScanner scanner(line);
    scanner.ignoreWhitespace();
    std::string time = scanner.nextToken();
    //int time_ = std::stoi(time.substr(1, time.size() - 2));
    std::cout << time << " ";
    std::string command = scanner.nextToken();
    if (command == "add_user")
    {
        std::string cur_username, username, passward, name, mailAddr = "";
        int privilege = 0;
        for (int i = 0; i < 6; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'c')
            {
                cur_username = scanner.nextToken();
            }
            else if (argument[1] == 'u')
            {
                username = scanner.nextToken();
            }
            else if (argument[1] == 'p')
            {
                passward = scanner.nextToken();
            }
            else if (argument[1] == 'n')
            {
                name = scanner.nextToken();
            }
            else if (argument[1] == 'm')
            {
                mailAddr = scanner.nextToken();
            }
            else if (argument[1] == 'g')
            {
                privilege = std::stoi(scanner.nextToken());
            }
        }
        //std::cout << cur_username << " " << username << " " << passward << " " << name << " " << mailAddr << " " << privilege << std::endl;
        user.add_user(cur_username.c_str(), username.c_str(), passward.c_str(), name.c_str(), mailAddr.c_str(), privilege);
    }
    else if (command == "login")
    {
        std::string username, passward = "";
        for (int i = 0; i < 2; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'u')
            {
                username = scanner.nextToken();
            }
            else if (argument[1] == 'p')
            {
                passward = scanner.nextToken();
            }
        }
        //std::cout << username << " " << passward << std::endl;
        user.login(username.c_str(), passward.c_str());
    }
    else if (command == "logout")
    {
        std::string username = "";
        scanner.nextToken();
        username = scanner.nextToken();
        user.logout(username.c_str());
    }
    else if (command == "query_profile")
    {
        std::string cur_username, username;
        for (int i = 0; i < 2; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'c')
            {
                cur_username = scanner.nextToken();
            }
            else if (argument[1] == 'u')
            {
                username = scanner.nextToken();
            }
        }
        //std::cout << cur_username << " " << username << std::endl;
        user.query_profile(cur_username.c_str(), username.c_str());
    }
    else if (command == "modify_profile")
    {
        std::string cur_username, username, name, mailAddr, passward = "";
        int privilege = -1;
        
        for (int i = 0; i < 6; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'c')
            {
                cur_username = scanner.nextToken();
            }
            else if (argument[1] == 'u')
            {
                username = scanner.nextToken();
            }
            else if (argument[1] == 'p')
            {
                passward = scanner.nextToken();
            }
            else if (argument[1] == 'n')
            {
                name = scanner.nextToken();
            }
            else if (argument[1] == 'm')
            {
                mailAddr = scanner.nextToken();
            }
            else if (argument[1] == 'g')
            {
                privilege = std::stoi(scanner.nextToken());
            }
        }
        user.modify_profile(cur_username.c_str(), username.c_str(), passward.c_str(), name.c_str(), mailAddr.c_str(), privilege);
    }
    else if (command == "add_train")
    {
        std::string trainID, startTime, saleDate = "";
        std::string stations[100], travelTime[100];
        int stationNum, seatNum, prices[100], stopoverTime[100];
        char type;
        for (int i = 0; i < 10; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'i')
            {
                trainID = scanner.nextToken();
            }
            else if (argument[1] == 'n')
            {
                stationNum = std::stoi(scanner.nextToken());
            }
            else if (argument[1] == 'm')
            {
                seatNum = std::stoi(scanner.nextToken());
            }
            else if (argument[1] == 's')
            {
                std::istringstream ss(scanner.nextToken());
                std::string token;
                int i = 0;
                while(std::getline(ss, token, '|'))
                {
                    stations[i] = token;
                    i++;
                }
            }
            else if (argument[1] == 'p')
            {
                std::istringstream ss(scanner.nextToken());
                std::string token;
                int i = 0;
                while(std::getline(ss, token, '|'))
                {
                    prices[i] = std::stoi(token);
                    i++;
                }
            }
            else if (argument[1] == 'x')
            {
                startTime = scanner.nextToken();
            }
            else if (argument[1] == 't')
            {
                std::istringstream ss(scanner.nextToken());
                std::string token;
                int i = 0;
                while(std::getline(ss, token, '|'))
                {
                    travelTime[i] = token;
                    i++;
                }
            }
            else if (argument[1] == 'o')
            {
                std::istringstream ss(scanner.nextToken());
                std::string token;
                int i = 0;
                while(std::getline(ss, token, '|'))
                {
                    stopoverTime[i] = std::stoi(token);
                    i++;
                }
            }
            else if (argument[1] == 'd')
            {
                saleDate= scanner.nextToken();
            }
            else if (argument[1] == 'y')
            {
                type = scanner.nextToken()[0];
            }
        }
    }
    else if (command == "delete_train")
    {
        scanner.nextToken();
        std::string trainID = scanner.nextToken();
    }
    else if (command == "release_train")
    {
        scanner.nextToken();
        std::string trainID = scanner.nextToken();
    }
    else if (command == "query_train")
    {
        std::string trainID, date = "";
        for (int i = 0; i < 2; ++i)
        {
            std::string argument = scanner.nextToken();
            if (argument[1] == 'i')
            {
                trainID = scanner.nextToken();
            }
            else if (argument[1] == 'd')
            {
                date = scanner.nextToken();
            }
        }
    }
    else if (command == "query_ticket")
    {
        std::string date = scanner.nextToken();
        std::string from = scanner.nextToken();
        std::string to = scanner.nextToken();
        char type = scanner.nextToken()[0];
        // Train::queryTicket(date, from, to, type);
    }
    else if (command == "query_transfer")
    {
        std::string date = scanner.nextToken();
        std::string from = scanner.nextToken();
        std::string to = scanner.nextToken();
        // Train::queryTransfer(date, from, to);
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
        // Train::buyTicket(date, trainID, from, to, ticketKind, ticketNum);
    }
    else if (command == "query_order")
    {
        std::string date = scanner.nextToken();
        // Train::queryOrder(date);
    }
    else if (command == "refund_ticket")
    {
        std::string date = scanner.nextToken();
        int ticketNum = std::stoi(scanner.nextToken());
        // Train::refundTicket(date, ticketNum);
    }
    else if (command == "clean")
    {

    }
    else if (command == "exit")
    {
        exit(0);
    }
    else
    {
        throw std::invalid_argument("Invalid command\n");
    }
}

int main()
{
    // freopen("testcases/basic_1/1.in", "r", stdin);
    // freopen("out", "w", stdout);
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