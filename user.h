#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
using namespace std;

class User
{
public:

    // pre: none
    // post: default user constructed
    User();

    // pre: none
    // post: user constructed
    User(std::size_t id, std::string name, int year, int zip, std::vector<std::size_t> friends);

    // pre: none
    // post: add the id to friend's list if not in there
    void add_friend(int id);

    // pre: none
    // post: remove the id to friend's list if in there
    void delete_friend(int id);

    // pre: none
    // post: return id
    std::size_t id() { return id_; }

    // pre: none
    // post: return name
    std::string name() { return name_; }

    // pre: none
    // post: return year
    int year() { return year_; }

    // pre: none
    // post: return zip
    int zip() { return zip_; }

    // pre: none
    // post: return pointer to friends
    std::vector<std::size_t>* friends() { return & friends_; }

    //pre: none
    //post: print the user's info
    void print();


    int depth, predecessor;
    bool visited;

private:

    std::size_t id_;
    std::string name_;
    int year_, zip_;
    std::vector<std::size_t> friends_;
};

#endif // USER_H
