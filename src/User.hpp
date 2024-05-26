#ifdef USER_HPP
#define USER_HPP

#include <string>
#include "../Database/BPT.hpp"
#include "../Database/MemoryRiver.hpp"

class Block_user
{
private:
    char username[20] = {'\0'};
    char passward[30] = {'\0'};
    char name[20] = {'\0'};
    char mailAddr[30] = {'\0'};
    int privilege = 0;
    bool islogin = false;
    int pos;

public:
    Block_user() = default;
    Block_user(const Block_user &other)
    {
        strcpy(this->username, other.username);
        strcpy(this->name, other.name);
        strcpy(this->passward, other.passward);
        strcpy(this->mailAddr, other.mailAddr);
        this->privilege = other.privilege;
        this->islogin = other.islogin;
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
        this->islogin = other.islogin;
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

    void setPos(int pos);
    int getPos();
    void setLogin(bool islogin);
    bool getLogin();
    void setPrivilege(int privilege);
    int getPrivilege();
    void setMailAddr(const char *mailAddr);
    const char *getMailAddr();
    void setName(const char *name);
    const char *getName();
    void setPassward(const char *passward);
    const char *getPassward();
};

class User
{
private:
    char username[20] = {'\0'};
    Block_user user;

public:
    User() = default;
    User(const User &other)
    {
        strcpy(this->username, other.username);
        this->user = other.user;
    }
    User &operator=(const User &other)
    {
        if (this == &other)
        {
            return *this;
        }
        strcpy(this->username, other.username);
        this->user = other.user;
        return *this;
    }
    bool operator==(const User &other)
    {
        return strcmp(this->username, other.username) == 0;
    }
    bool operator!=(const User &other)
    {
        return strcmp(this->username, other.username) != 0;
    }
    bool operator<(const User &other)
    {
        return strcmp(this->username, other.username) < 0;
    }
    bool operator>(const User &other)
    {
        return strcmp(this->username, other.username) > 0;
    }

    bool add_user(const char *cur_username = nullptr, const char *username, const char *passward, const char *mailAddr, int privilege = 10);
    bool login(const char *username, const char *passward);
    bool logout(const char *username);
    bool query_profile(const char *cur_username, const char *username);
    bool modify_profile(const char *cur_username, const char *username, const char *passward = nullptr, const char *name = nullptr, const char *mailAddr = nullptr, int privilege = -1);
    bool buy_ticket(const char *cur_username, const char *trainID, const char *from, const char *to, const char *date, int num, bool if_queue = false);
    bool query_order(const char *cur_username);
    bool refund_ticket(const char *cur_username, int orderID = 1);
};

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

static BPT<int, 500> users("users");                   // 用户全部信息索引
static BPT<order, 500> user_orders("user_orders");     // 用户订单信息
static std::map<string, bool> user_list;               // 用户列表
static std::map<string, int> user_map;                 // 用户名到用户信息索引的映射
static MemoryRiver<Block_user> userRiver("userRiver"); // 用户信息存储

#endif