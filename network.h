#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include "user.h"

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

    // pre:none
    // post: returns pointer to user with id if it exists, otherwise NULL
    User * get_user(std::size_t id);

    // pre: none
    // post: returns a vector of id's of the path to "from" and "to" if it exists, otherwise an empty vector
    std::vector<int> shortest_path(int from, int to);

    // pre: none
    // post: returns a vector of vectors storing id's, each vector of id's is a disconnected group
    std::vector<std::vector<int>> groups();

    // pre: none
    // post: return all id's with the highest mutual friendship
    std::vector<int> suggest_friends(int who, int & score);

    // pre: none
    // post: run a single depth first search check, if a user at the distance is found then return a
    //       vector of the path to that and also set "to" to the id of them
    std::vector<int> distance_user(int from, int& to, int distance);

    std::string displayPosts(std::string name, int howmany);

    std::string displayDM(std::string from, std::string to, int howmany);

    void addPost(std::string who, std::string message, int likes, int id);

    void addDM(std::string who, std::string message, int likes, int id, std::string recipient);

private:

    std::vector<User *> users_;
};

#endif // NETWORK_H
