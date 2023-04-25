#include "user.h"
#include <algorithm>


// pre: none
// post: default user constructed
User::User(){
    friends_ = std::vector<std::size_t>();
    messages_ = std::vector<Post *>();
}

// pre: none
// post: user constructed
User::User(std::size_t id, std::string name, int year, int zip, std::vector<std::size_t> friends)
{
    id_ = id;
    name_ = name;
    year_ = year;
    zip_ = zip;
    friends_ = friends;
    messages_ = std::vector<Post *>();
}

// pre: none
// post: add the id to friend's list if not in there
void User::add_friend(int id)
{
    if(id != id_ && std::find(friends_.begin(), friends_.end(), id) == friends_.end())
        friends_.push_back(id);
}

// pre: none
// post: remove the id to friend's list if in there
void User::delete_friend(int id)
{
    std::vector<std::size_t>::iterator index = std::find(friends_.begin(), friends_.end(), id);
    if(index != friends_.end())
        friends_.erase(index);
        // remove(friends_.begin(), friends_.end(), id);
}

//pre: none
//post: print the user's info
void User::print()
{
    std::cout<<id_<<' '<<name_<<' '<<year_<<' '<<zip_<<std::endl;
}

void User::addPost(Post * post)
{
    messages_.push_back(post);
}

std::string User::displayPosts(int howmany)
{
    if(howmany == 0)
        return "";
    std::string result(""), new_line;
    int counter(0);
    for(int i = messages_.size() - 1; i >= 0; --i)
    {
        new_line = messages_[i]->displayPost();
        // std::cout<<"DEBUG: "<<new_line<<std::endl;
        if(std::find(new_line.begin(), new_line.end(), '$') == new_line.end())
        {

            result += new_line + "\n\n";
            ++counter;
            if(counter == howmany)
                break;
        }
    }
    // std::cout<<"DEBUG: "<<result<<std::endl;
    return result;
}


std::string User::displayDMs(int who, std::string name, int howmany)
{
    // std::cout<<"DEBUG: displaying DMs"<<std::endl;
    if(howmany == 0)
        return "";
    std::string result(""), new_line;
    int counter(0), recipient_start;
    for(int i = messages_.size() - 1; i >= 0; --i)
    {
        new_line = messages_[i]->displayPost();
        // std::cout<<"DEBUG: checking this post \n"<< new_line << std::endl;
        if(*(new_line.end() - 1) == '$')
        {
            recipient_start = new_line.size() - 2;
            while(new_line[recipient_start - 1] != '$')
            {
                --recipient_start;
            }
            // assume all DM's in messages_ are authored by this User?
            // I assume "name" is name of "who"
            // std::cout<<"DEBUG: recipient is " << std::stoi(new_line.substr(recipient_start, (new_line.size() - 1) - recipient_start)) << std::endl;
            bool recipient_is_who = (std::stoi(new_line.substr(recipient_start, (new_line.size() - 1) - recipient_start)) == who);
            if(recipient_is_who)
            {
                result += "From: " + name_ + "\n";
                result += new_line.substr(0, recipient_start - 1) + "\n\n";
                ++counter;
                if(counter == howmany)
                    break;
            }

        }
    }
    // std::cout<<"DEBUG: "<<result<<std::endl;
    return result;
}

int User::num_messages()
{
    return messages_.size();
}
