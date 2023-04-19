#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include "user.h"
using namespace std;


class Network
{
public:

    // pre: none
    // post: construct default network
    Network();

    // pre: none
    // post: add user to list of users
    void add_user(User & user);

    // pre: no self connection
    // post: add connection if not existing
    int add_connection(std::string s1, std::string s2);

    // pre: none
    // post: remove connection if it exists
    int remove_connection(std::string s1, std::string s2);

    // pre: none
    // post: return id of the specified person
    int get_id(std::string name);

    // pre: file can be loaded
    // post: initialize network to file
    int read_friends(char* fname);

    // pre: file can be loaded
    // post: output the network data to file
    int write_friends(char* fname);

    // pre: none
    // post: return number of users
    std::size_t num_users();

    // pre: none
    // post: print all the users' info
    void print();

    // pre: none
    // post: print friends info if the user exists
    int print_friends(std::string name);

    User * get_user(std::size_t id);

    vector<int> shortest_path(int from, int to);

    vector<vector<int>> groups();

private:

    vector<User *> users_;
};

#endif // NETWORK_H
