#include "user.h"
#include <algorithm>


// pre: none
// post: default user constructed
User::User(){
    friends_ = std::vector<size_t>();
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
    cout<<id_<<' '<<name_<<' '<<year_<<' '<<zip_<<endl;
//    cout<<"ID: "<<id_<<endl;
//    cout<<"   Name: "<<name_<<endl;
//    cout<<"   Birth: "<<year_<<endl;
//    cout<<"   Zip: "<<zip_<<endl;
//    cout<<"   Friends: ";
//    if(friends_.empty())
//        cout<<"None"<<endl;
//    else
//    {
//        for(auto e: friends_)
//            cout<<e<<" ";
//        cout<<endl;
//    }
}
