#include "network.h"
#include <cassert>
#include <fstream>
#include <algorithm>
#include <queue>

// pre: none
// post: construct default network
Network::Network(){}

// pre: none
// post: add user to list of users
void Network::add_user(User & user)
{
    users_.push_back(& user);
}

// pre: no self connection
// post: add connection if not existing
int Network::add_connection(std::string s1, std::string s2)
{
    assert(s1 != s2);
    User * u1, * u2;
    u1 = u2 = NULL;

    for(auto e: users_)
    {
        if(e->name() == s1)
            u1 = e;
        if(e->name() == s2)
            u2 = e;
    }

    if(u1 == NULL || u2 == NULL)
        return -1;

    u1->add_friend(u2->id());
    u2->add_friend(u1->id());
    return 0;
}

// pre: none
// post: remove connection if it exists
int Network::remove_connection(std::string s1, std::string s2)
{
    User * u1, * u2;
    u1 = u2 = NULL;

    for(auto e: users_)
    {
        if(e->name() == s1)
            u1 = e;
        if(e->name() == s2)
            u2 = e;
    }

    if(u1 == NULL || u2 == NULL)
        return -1;

    u1->delete_friend(u2->id());
    u2->delete_friend(u1->id());
    return 0;
}

// pre: none
// post: return id of the specified person
int Network::get_id(std::string name)
{
    User * u;
    u = NULL;

    for(auto e: users_)
        if(e->name() == name)
            u = e;

    if (u == NULL)
        return -1;

    return u->id();
}

// pre: file can be loaded
// post: initialize network to file
int Network::read_friends(char* fname)
{
    ifstream is;
    is.open(fname);
    if(is.fail()){
        cout<<"Input file opening failed!"<<endl;
        return -1;
    }

    users_.clear(); // reset network

    std::string id, name, birth, zip, friends, temp_friend_id;
    std::vector<std::size_t> friends_vector;

    getline(is, id); // set ID to dummy value to ignore first line

    User temp;

    while(getline(is, id) &&
          getline(is, name) &&
          getline(is, birth) &&
          getline(is, zip) &&
          getline(is, friends)
          )
    {
        friends_vector.clear();
        std::size_t counter = 1;
        while(counter < friends.size() - 1) // has -1 because writing file creates space at the end
        {
            temp_friend_id = &friends[counter];
            ++counter;
            while(friends[counter] != ' ')
            {
                temp_friend_id += &friends[counter];
                ++counter;
            }
            friends_vector.push_back(std::stoi(temp_friend_id));
        }

        users_.push_back(new User(std::stoi(id), &(name[1]), std::stoi(birth), std::stoi(zip), friends_vector));
    }

    return 0;
}

// pre: file can be loaded
// post: output the network data to file
int Network::write_friends(char* fname)
{
    ofstream os;
    os.open(fname);
    if(os.fail()){
        cout<<"Output file opening failed!"<<endl;
        return -1;
    }

    os<<users_.size()<<endl;
    for(auto e: users_)
    {
        os<<e->id()<<endl;
        os<<"\t"<<e->name()<<endl;
        os<<"\t"<<e->year()<<endl;
        os<<"\t"<<e->zip()<<endl;
        os<<"\t";
        for(auto f: *(e->friends()))
        {
            os<<f<<" ";
        }
        os<<endl;
    }

    os.close();
    return 0;
}

// pre: none
// post: return number of users
std::size_t Network::num_users()
{
    return users_.size();
}

// pre: none
// post: print all the users' info
void Network::print()
{
    //cout<<this->num_users()<<endl;
    for(auto u: users_)
        u->print();
}

// pre: none
// post: print friends info if the user exists
int Network::print_friends(std::string name)
{
    User * u;
    u = NULL;

    for(auto e: users_)
        if(e->name() == name)
            u = e;

    if (u == NULL)
        return -1;

    std::vector<std::size_t> friends_list = * u->friends();
    for(auto user: users_)
    {
        if(std::find(friends_list.begin(), friends_list.end(), user->id()) != friends_list.end())
            user->print();
    }
    return 0;
}

User* Network::get_user(std::size_t id)
{
    for(auto u: users_)
        if(u->id() == id)
            return u;
    return NULL;
}


std::vector<int> Network::shortest_path(int from, int to)
{
    if(from == to)
    {
        get_user(to)->depth = 0;
        return std::vector<int>{to};
    }

    // reset all user's predecessor to -1, marking them as unvisited
    for(auto & u: users_)
        u->visited = false;

    std::vector<int> path;
    std::queue<User*> q;

    q.push(get_user(from));
    q.front()->depth = 0;
    q.front()->visited = true; // means start
    //cout<<"DEBUG: add "<<from<<" with depth "<< 0 <<" and pred "<<-2<<endl;

    while(!q.empty())
    {
        User* u = q.front();
        q.pop();
        for(auto friend_id: * u->friends())
        {
            User* f = get_user(friend_id);
            if(f->id() == to)
            {
                //cout<<"DEBUG: found"<<endl;
                f->depth = u->depth + 1;
                f->predecessor = u->id();
                User* curr = f;
                while(curr->id() != from)
                {
                    //cout<<"DEBUG: "<<curr->predecessor<<endl;
                    path.push_back(curr->id());
                    curr = get_user(curr->predecessor);
                }
                path.push_back(from);
                std::reverse(path.begin(),path.end());
                return path;
            }
            if(f->visited == false)
            {
                f->visited = true;
                f->depth = u->depth + 1;
                f->predecessor = u->id();
                //cout<<"DEBUG: add "<<f->id()<<" with depth "<< f->depth <<" and pred "<<f->predecessor<<endl;
                q.push(f);
            }
        }
    }
    //cout<<"DEBUG: not found"<<endl;
    return std::vector<int>();
}

std::vector<vector<int>> Network::groups()
{
    // initialization
    for(auto & u: users_)
        u->visited = false;
    std::queue<User*> q;
    std::vector<vector<int>> groups;
    std::size_t component = 0;


    for(auto & user: users_)
    {
        if(user->visited == false)
        {
            user->visited = true;
            q.push(user);
            groups.push_back(std::vector<int>());

            while(!q.empty())
            {
                User* u = q.front();
                groups[component].push_back(u->id());
                q.pop();

                for(auto friend_id: * u->friends())
                {
                    User* f = get_user(friend_id);
                    if(f->visited == false)
                    {
                        f->visited = true;
                        q.push(f);
                    }
                }
            }
            ++component;
        }
    }

    return groups;
}
