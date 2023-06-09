#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include "post.h"

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

    //pre: none
    //post: add post user's message list
    void addPost(Post * post);

    //pre: none
    // post: display the recent posts of the user up to the specified amount
    std::string displayPosts(int howmany);

    //pre: none
    // post: display the recent DMs of the user up to the specified amount
    std::string displayDMs(int who, std::string name, int howmany);

    //pre: none
    //post: returns the number of messages
    int num_messages(){ return messages_.size(); }

    int depth, predecessor, score;
    bool visited;

private:

    std::size_t id_;
    std::string name_;
    int year_, zip_;
    std::vector<std::size_t> friends_;
    std::vector<Post *> messages_;
};

#endif // USER_H
