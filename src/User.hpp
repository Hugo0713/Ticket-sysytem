#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <iostream>
#include <cstring>
#include <map>
#include "../Database/BPT.hpp"
#include "../Database/MemoryRiver.hpp"

struct order
{
    int status; // 0: success 1: pending 2: refunded
    int time;
    char trainID[20] = {'\0'};
    char from[40] = {'\0'};
    char to[40] = {'\0'};
    char leaving_time[8] = {'\0'};
    char arriving_time[8] = {'\0'};
    int price = 0;
    int num = 0;
};

class Block_user
{
private:
public:
    char username[20] = {'\0'};
    char passward[30] = {'\0'};
    char name[20] = {'\0'};
    char mailAddr[30] = {'\0'};
    int privilege = 0;
    int pos;

public:
    Block_user() = default;
    Block_user(const char *username, const char *passward, const char *name, const char *mailAddr, int privilege, int pos)
    {
        strcpy(this->username, username);
        strcpy(this->name, name);
        strcpy(this->passward, passward);
        strcpy(this->mailAddr, mailAddr);
        this->privilege = privilege;
        this->pos = pos;
    }
    Block_user(const Block_user &other)
    {
        strcpy(this->username, other.username);
        strcpy(this->name, other.name);
        strcpy(this->passward, other.passward);
        strcpy(this->mailAddr, other.mailAddr);
        this->privilege = other.privilege;
        this->pos = other.pos;
    }
    Block_user &operator=(const Block_user &other)
    {
        if (this == &other)
        {
            return *this;
        }
        strcpy(this->username, other.username);
        strcpy(this->name, other.name);
        strcpy(this->passward, other.passward);
        strcpy(this->mailAddr, other.mailAddr);
        this->privilege = other.privilege;
        this->pos = other.pos;
        return *this;
    }
    bool operator==(const Block_user &other)
    {
        return strcmp(this->username, other.username) == 0;
    }
    bool operator!=(const Block_user &other)
    {
        return strcmp(this->username, other.username) != 0;
    }
    bool operator<(const Block_user &other)
    {
        return strcmp(this->username, other.username) < 0;
    }
    bool operator>(const Block_user &other)
    {
        return strcmp(this->username, other.username) > 0;
    }
};

class User
{
private:
    static BPT<int, 500> users;                  // 用户全部信息索引
    static BPT<order, 500> user_orders;          // 用户订单信息
    static std::map<string, int> user_list;      // 用户列表 name->privilege
    static std::map<string, int> user_map;       // 用户名到用户信息索引的映射
    static MemoryRiver<Block_user, 1> userRiver; // 用户信息存储,info存位置

public:
    User()
    {
        users.initialise("users");
        user_orders.initialise("user_orders");
        userRiver.initialise("userRiver");
    }
    // User &operator=(const User &other)
    // {
    //     if (this == &other)
    //     {
    //         return *this;
    //     }
    //     users = other.users;
    //     user_orders = other.user_orders;
    //     user_list = other.user_list;
    //     user_map = other.user_map;
    //     userRiver = other.userRiver;
    //     return *this;
    // }

    void add_user(const char *cur_username, const char *username, const char *passward, const char *name, const char *mailAddr, int privilege = 10);
    void login(const char *username, const char *passward);
    void logout(const char *username);
    void query_profile(const char *cur_username, const char *username);
    void modify_profile(const char *cur_username, const char *username, const char *passward = nullptr, const char *name = nullptr, const char *mailAddr = nullptr, int privilege = -1);
    bool buy_ticket(const char *cur_username, const char *trainID, const char *from, const char *to, const char *date, int num, bool if_queue = false);
    bool query_order(const char *cur_username);
    bool refund_ticket(const char *cur_username, int orderID = 1);
};

// void User::add_user(const char *cur_username, const char *username, const char *passward, const char *name, const char *mailAddr, int privilege)
// {
//     // std::cout << cur_username << " " << username << " " << passward << " " << name << " " << mailAddr << " " << privilege << std::endl;
//     int user_num;
//     userRiver.get_info(user_num, 1);
//     // std::cout << user_num << std::endl;
//     if (user_num == 0)
//     {
//         user_num = 1;
//         userRiver.write_info(user_num, 1);
//         Block_user new_user(username, passward, name, mailAddr, 10, user_num);
//         userRiver.write(new_user, user_num);
//         users.insert(username, user_num);
//         // std::cout << username << users.Find1(username) << std::endl;
//         user_map[username] = user_num;
//         std::cout << "0" << std::endl;
//         return;
//     }
//     if (user_list.find(cur_username) == user_list.end() || user_list[cur_username] <= privilege || users.Find1(username))
//     {
//         std::cout << "-1" << std::endl;
//         return;
//     }
//     user_num++;
//     Block_user new_user(username, passward, name, mailAddr, privilege, user_num);
//     userRiver.write_info(user_num, 1);
//     userRiver.write(new_user, user_num);
//     users.insert(username, user_num);
//     user_map[username] = user_num;
//     std::cout << "0" << std::endl;
//     return;
// }

// void User::login(const char *username, const char *passward)
// {
//     // std::cout << username << std::endl;
//     int pos = users.Find2(username);
//     // std::cout << users.Find1(username) <<  pos << std::endl;
//     if (pos == -1 || user_list.find(username) != user_list.end())
//     {
//         std::cout << "-1" << std::endl;
//         return;
//     }
//     Block_user user;
//     userRiver.read(user, pos);
//     if (strcmp(user.passward, passward) == 0)
//     {
//         user_list[username] = user.privilege;
//         std::cout << user_list[username] << std::endl;
//         // 遍历user_list
//         for (auto it = user_list.begin(); it != user_list.end(); ++it)
//         {
//             std::cout << it->first << " " << it->second << std::endl;
//         }
//         std::cout << "0" << std::endl;
//         return;
//     }
//     std::cout << "-1" << std::endl;
//     return;
// }

// void User::logout(const char *username)
// {
//     if (user_list.find(username) == user_list.end())
//     {
//         std::cout << "-1" << std::endl;
//         return;
//     }
//     user_list.erase(username);
//     std::cout << "0" << std::endl;
//     return;
// }

// void User::query_profile(const char *cur_username, const char *username)
// {
//     int pos = users.Find2(username);
//     Block_user user;
//     userRiver.read(user, pos);
//     for (auto it = user_list.begin(); it != user_list.end(); ++it)
//     {
//         std::cout << it->first << " " << it->second << std::endl;
//     }
//     // std::cout << pos << " " << user_list[cur_username] << " " << user.privilege << std::endl;
//     if (user_list.find(cur_username) == user_list.end() || pos == 0 || ((user_list[cur_username] <= user.privilege) && strcmp(cur_username, username) != 0))
//     {
//         std::cout << "-1\n";
//         return;
//     }
//     std::cout << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege << std::endl;
// }

// void User::modify_profile(const char *cur_username, const char *username, const char *passward, const char *name, const char *mailAddr, int privilege)
// {
//     int pos = users.Find2(username);
//     Block_user user;
//     userRiver.read(user, pos);
//     if (user_list.find(cur_username) == user_list.end() || pos == -1 || privilege >= user_list[cur_username] || ((user_list[cur_username] <= user.privilege) && strcmp(cur_username, username) != 0))
//     {
//         std::cout << "-1\n";
//         return;
//     }
//     if (passward != nullptr)
//     {
//         strcpy(user.passward, passward);
//     }
//     if (name != nullptr)
//     {
//         strcpy(user.name, name);
//     }
//     if (mailAddr != nullptr)
//     {
//         strcpy(user.mailAddr, mailAddr);
//     }
//     if (privilege != -1)
//     {
//         user.privilege = privilege;
//     }
//     userRiver.write(user, pos);
//     if (user_list.find(username) != user_list.end())
//     {
//         user_list[username] = user.privilege;
//     }
//     std::cout << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege << std::endl;
// }

#endif