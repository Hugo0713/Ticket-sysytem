#include "User.hpp"
#include <string>

// 定义静态成员
BPT<int, 500> User::users;
BPT<order, 500> User::user_orders;
std::map<std::string, int> User::user_list;
std::map<std::string, int> User::user_map;
MemoryRiver<Block_user, 1> User::userRiver;

void User::add_user(const char *cur_username, const char *username, const char *passward, const char *name, const char *mailAddr, int privilege)
{
    // std::cout << cur_username << " " << username << " " << passward << " " << name << " " << mailAddr << " " << privilege << std::endl;
    int user_num;
    userRiver.get_info(user_num, 1);
    // std::cout << user_num << std::endl;
    if (user_num == 0)
    {
        user_num = 1;
        userRiver.write_info(user_num, 1);
        Block_user new_user(username, passward, name, mailAddr, 10, user_num);
        userRiver.write(new_user, user_num);
        users.insert(username, user_num);
        // std::cout << username << users.Find1(username) << std::endl;
        user_map[username] = user_num;
        std::cout << "0" << std::endl;
        return;
    }
    if (user_list.find(cur_username) == user_list.end() || user_list[cur_username] <= privilege || users.Find1(username))
    {
        std::cout << "-1" << std::endl;
        return;
    }
    user_num++;
    Block_user new_user(username, passward, name, mailAddr, privilege, user_num);
    userRiver.write_info(user_num, 1);
    userRiver.write(new_user, user_num);
    users.insert(username, user_num);
    user_map[username] = user_num;
    std::cout << "0" << std::endl;
    return;
}

void User::login(const char *username, const char *passward)
{
    // std::cout << username << std::endl;
    int pos = users.Find2(username);
    // std::cout << users.Find1(username) <<  pos << std::endl;
    if (pos == -1 || user_list.find(username) != user_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    Block_user user;
    userRiver.read(user, pos);
    if (strcmp(user.passward, passward) == 0)
    {
        user_list[username] = user.privilege;
        //std::cout << user_list[username] << std::endl;
        // // 遍历user_list
        // for (auto it = user_list.begin(); it != user_list.end(); ++it)
        // {
        //     std::cout << it->first << " " << it->second << std::endl;
        // }
        std::cout << "0" << std::endl;
        return;
    }
    std::cout << "-1" << std::endl;
    return;
}

void User::logout(const char *username)
{
    if (user_list.find(username) == user_list.end())
    {
        std::cout << "-1" << std::endl;
        return;
    }
    user_list.erase(username);
    std::cout << "0" << std::endl;
    return;
}

void User::query_profile(const char *cur_username, const char *username)
{
    int pos = users.Find2(username);
    Block_user user;
    userRiver.read(user, pos);
    // for (auto it = user_list.begin(); it != user_list.end(); ++it)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    // std::cout << pos << " " << user_list[cur_username] << " " << user.privilege << std::endl;
    if (user_list.find(cur_username) == user_list.end() || pos == 0 || ((user_list[cur_username] <= user.privilege) && strcmp(cur_username, username) != 0))
    {
        std::cout << "-1\n";
        return;
    }
    std::cout << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege << std::endl;
}

void User::modify_profile(const char *cur_username, const char *username, const char *passward, const char *name, const char *mailAddr, int privilege)
{
    int pos = users.Find2(username);
    Block_user user;
    userRiver.read(user, pos);
    if (user_list.find(cur_username) == user_list.end() || pos == -1 || privilege >= user_list[cur_username] || ((user_list[cur_username] <= user.privilege) && strcmp(cur_username, username) != 0))
    {
        std::cout << "-1\n";
        return;
    }
    if (passward[0] != '\0')
    {
        strcpy(user.passward, passward);
    }
    if (name[0] != '\0')
    {
        strcpy(user.name, name);
    }
    if (mailAddr[0] != '\0')
    {
        strcpy(user.mailAddr, mailAddr);
    }
    if (privilege != -1)
    {
        user.privilege = privilege;
    }
    userRiver.write(user, pos);
    if (user_list.find(username) != user_list.end())
    {
        user_list[username] = user.privilege;
    }
    std::cout << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege << std::endl;
}
