#include "network.h"
#include <cassert>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include "post.h"
#include "directmessage.h"

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

    std::ifstream is;
    is.open(fname);
    if(is.fail()){
        std::cout<<"Input file opening failed!"<<std::endl;
        return -1;
    }

    users_.clear(); // reset network

    std::string id, name, birth, zip, friends, temp_friend_id;
    std::vector<std::size_t> friends_vector;

    getline(is, id); // set ID to dummy value to ignore first line

    while(getline(is, id) &&
          getline(is, name) &&
          getline(is, birth) &&
          getline(is, zip) &&
          getline(is, friends)
          )
    {
        friends_vector.clear();
        std::size_t counter = 1;
        while(counter < friends.size() - 2) // has -1 because writing file creates space at the end
        {
            char c = friends[counter];
            temp_friend_id = "";
            temp_friend_id += c;
            ++counter;
            while(friends[counter] != ' ')
            {
                c = friends[counter];
                temp_friend_id += c;
                ++counter;
            }
            friends_vector.push_back(std::stoi(temp_friend_id));
        }
        name = name.substr(1, name.size()-2);
        users_.push_back(new User(std::stoi(id), name, std::stoi(birth), std::stoi(zip), friends_vector));
    }
    is.close();
    return 0;
}

// pre: file can be loaded
// post: output the network data to file
int Network::write_friends(char* fname)
{
    std::ofstream os;
    os.open(fname);
    if(os.fail()){
        std::cout<<"Output file opening failed!"<<std::endl;
        return -1;
    }

    os<<users_.size()<<std::endl;
    for(auto e: users_)
    {
        os<<e->id()<<std::endl;
        os<<"\t"<<e->name()<<std::endl;
        os<<"\t"<<e->year()<<std::endl;
        os<<"\t"<<e->zip()<<std::endl;
        os<<"\t";
        for(auto f: *(e->friends()))
        {
            os<<f<<" ";
        }
        os<<std::endl;
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

// pre:none
// post: returns pointer to user with id if it exists, otherwise NULL
User* Network::get_user(std::size_t id)
{
    for(auto u: users_)
        if(u->id() == id)
            return u;
    return NULL;
}

// pre: none
// post: returns a vector of id's of the path to "from" and "to" if it exists, otherwise an empty vector
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

// pre: none
// post: returns a vector of vectors storing id's, each vector of id's is a disconnected group
std::vector<std::vector<int>> Network::groups()
{
    // initialization
    for(auto & u: users_)
        u->visited = false;
    std::queue<User*> q;
    std::vector<std::vector<int>> groups;
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

// pre: none
// post: return all id's with the highest mutual friendship
std::vector<int> Network::suggest_friends(int who, int& score)
{
    score = -1;

    for(auto & u: users_)
    {
        u->visited = false;
    }

    std::vector<User *> all_depth_2;
    std::queue<User*> q;

    if(get_user(who) == NULL)
        return std::vector<int>();

    q.push(get_user(who));
    q.front()->depth = 0;
    q.front()->visited = true;

    // perform bfs, but it will stop at users of depth 2
    while(!q.empty())
    {
        User* u = q.front();
        q.pop();
        for(auto friend_id: * u->friends())
        {
            User* f = get_user(friend_id);
            if(f->visited == false)
            {
                f->visited = true;
                f->depth = u->depth + 1;
                if(f->depth == 2)
                {
                    all_depth_2.push_back(f);
                    f->score = 1;
                    if(score == -1)
                        score = 1;
                }
                else
                    q.push(f);
            }
            else if(f->visited == true && f->depth == 2)
            {
                f->score += 1;
                if(f->score > score)
                    score = f->score;
            }
        }
    }

    // if no suggestions exists (score will never have been set, thus it stayed -1)
    if(score == -1)
    {
        return std::vector<int>();
    }

    // cull suggestions to those only of the highest score
    std::vector<int> suggested;
    for(auto e: all_depth_2)
    {
        if(e->score == score)
            suggested.push_back(e->id());
    }
    return suggested;
}


// pre: none
// post: run a single depth first search check, if a user at the distance is found then return a
//       vector of the path to that and also set "to" to the id of them
std::vector<int> Network::distance_user(int from, int& to, int distance)
{
    to = -1;

    if(distance == 0 || distance == 1)
        return std::vector<int>();

    for(auto & u: users_)
    {
        u->visited = false;
        u->depth = 0;
    }

    std::stack<User *> s;
    User * root = get_user(from);

    if(root == NULL)
        return std::vector<int>();

    root->depth = 0;
    root->visited = true;
    s.push(root);
    // cout<<"ADD: "<<root->name()<<" added to stack with depth "<<root->depth<<endl;

    while(!s.empty())
    {
        User* u = s.top();
        s.pop();
        for(auto friend_id: * u->friends())
        {
            User* f = get_user(friend_id);
            if(!f->visited)
            {
                f->depth = u->depth + 1;
                f->predecessor = u->id();
                // cout<<"ADD: "<<f->name()<<" added to stack with depth "<<f->depth<<endl;
                if(f->depth == distance && std::find(root->friends()->begin(), root->friends()->end(), f->id()) == root->friends()->end())
                {
                    // if it matches the distance and is not already a friend
                    to = f->id();
                    // cout<<"DEBUG: found a match with name "<<get_user(to)->name()<<endl;
                    s = std::stack<User *>();
                    break;
                }
                else if(f->depth != distance)
                {
                    // will not stop adding to stack when depth is the distance
                    f->visited = true;
                    s.push(f);
                }
            }
        }
    }

    // cout<<"DEBUG: stack finished"<<endl;
    // if no match found
    if(to == -1)
        return std::vector<int>();

    // else create path
    std::vector<int> path;
    User* curr = get_user(to);
    while(curr->id() != from)
    {
        path.push_back(curr->id());
        curr = get_user(curr->predecessor);
    }
    path.push_back(from);
    std::reverse(path.begin(), path.end());
    return path;
}


std::string Network::displayPosts(std::string name, int howmany)
{
    for(auto & u: users_)
        if(u->name() == name)
        {
            return u->displayPosts(howmany);
        }
    return "";
}

std::string Network::displayDM(std::string from, std::string to, int howmany)
{
    for(auto & u: users_)
        if(u->name() == from)
        {
            int author_id = get_id(to);
            if(author_id == -1)
                return "";
            return u->displayDMs(author_id, to, howmany);
        }
    return "";
}

void Network::addPost(std::string who, std::string message, int likes, int id)
{
    int author_id = get_id(who);
    if(author_id == -1)
    {
        return;
    }
    for(auto & u: users_)
        if(u->name() == who)
        {
            u->addPost(new Post(id, author_id, likes, message));
            break;
        }
}

void Network::addDM(std::string who, std::string message, int likes, int id, std::string recipient)
{
    int author_id = get_id(who);
    int recipient_id = get_id(recipient);
    if(author_id == -1 || recipient_id == -1)
    {
        return;
    }
    for(auto & u: users_)
        if(u->name() == who)
        {
            u->addPost(new DirectMessage(id, author_id, likes, message, recipient_id));
            // std::cout<<"DEBUG: "<< u->displayDMs(recipient_id, "filler", 1) <<std::endl;
            break;
        }
}

int Network::read_posts(char* fname)
{
    // std::cout<<"DEBUG: start"<<std::endl;
    std::ifstream is;
    is.open(fname);
    if(is.fail()){
        std::cout<<"Input file opening failed!"<<std::endl;
        return -1;
    }

    std::string message_id, message_text, author_id, likes, is_DM, recipient_id;
    User * author, * recipient;

    getline(is, message_id); // set ID to dummy value to ignore first line

    while(getline(is, message_id) &&
          getline(is, message_text) &&
          getline(is, author_id) &&
          getline(is, likes) &&
          getline(is, is_DM)
          )
    {
        message_text = message_text.substr(1, message_text.size() - 1);

//        std::cout<<"DEBUG: 0 "<<message_id<<std::endl;
//        std::cout<<"DEBUG: 1 "<<message_text<<std::endl;
//        std::cout<<"DEBUG: 2 "<<author_id<<std::endl;
//        std::cout<<"DEBUG: 3 "<<likes<<std::endl;
//        std::cout<<"DEBUG: 4 "<<is_DM<<std::endl;


        if(is_DM.size() <= 1 || is_DM.substr(1) != "DM")
        {
            // std::cout<<"DEBUG: create post "<<std::endl;
            author = get_user(std::stoi(author_id));
            if(author != NULL)
                addPost(author->name(),
                        message_text,
                        std::stoi(likes),
                        std::stoi(message_id));
        }
        else
        {
            getline(is, recipient_id);
            // std::cout<<"DEBUG: 5 "<<recipient_id<<std::endl;
            // std::cout<<"DEBUG: create DM"<<std::endl;
            author = get_user(std::stoi(author_id));
            if(recipient_id != "\t")
                recipient = get_user(std::stoi(recipient_id));
            else
                recipient = NULL;
            if(!(author == NULL || recipient == NULL))
                addDM(author->name(),
                      message_text,
                      std::stoi(likes),
                      std::stoi(message_id),
                      recipient->name());
        }
    }
    is.close();
    // std::cout<<"DEBUG: end"<<std::endl;
    return 0;
}




