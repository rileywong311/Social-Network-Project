#include <iostream>
#include "network.h"
#include <sstream>
#include <cassert>
using namespace std;

#include "post.h"
#include "directmessage.h"

void test()
{
    Network n , n2;

    cout<<"---READ FRIENDS---"<<endl;
    n.read_friends("C://cygwin64//home//yelir//SocialNetwork//SocialNetwork//SN-2.txt");
    n2.read_friends("C://cygwin64//home//yelir//SocialNetwork//SocialNetwork//SN-2.txt");

    cout<<"---PRINT USERS---"<<endl;
    n.print();
    cout<<endl;

    cout<<"---TEST SHORTEST PATH---"<<endl;
    int from(0), to(123);
    std::vector<int> i = n.shortest_path(from, to);
    cout<<"Distance: "<<n.get_user(i[i.size()-1])->depth<<endl;
    for(std::size_t j = 0; j < i.size() - 1; ++j)
        cout<<n.get_user(i[j])->name()<<" -> ";
    cout<<n.get_user(i[i.size()-1])->name()<<endl;


    cout<<"\n---TEST DISJOINT SETS---"<<endl;
    std::vector<vector<int>> result = n.groups();
    for(std::size_t i = 0; i < result.size(); ++i)
    {
        cout<<"Set "<<i+1<<" => ";
        for(auto j: result[i])
            cout<<n.get_user(j)->name()<<", ";
        cout<<endl;
    }

    cout<<"---SUGGESTION---"<<endl;
    int s;
    std::vector<int> j = n.suggest_friends(12, s);
    if(s == -1)
        cout<<"None"<<endl;
    else
    {
        cout<<"The suggested friend(s) is/are:"<<endl;
        for(auto e: j)
        {
            cout<<n.get_user(e)->name()<<" with score "<<s<<endl;
        }
    }

    cout<<"---SUGGESTION---"<<endl;
    int to2;
    std::vector<int> distance = n.distance_user(5, to2, 5);
    if(to == -1)
        cout<<"No match was calculated"<<endl;
    else
    {
        cout<<"Found a match! "<<n.get_user(to)->name()<<": ";
        for(std::size_t j = 0; j < distance.size() - 1; ++j)
            cout<<n.get_user(distance[j])->name()<<" -> ";
        cout<<n.get_user(distance[distance.size()-1])->name()<<endl;
        cout<<endl;
    }

    cout<<"---CREATE POST---"<<endl;
    Post p(0, 1, 53, "Test post");
    cout<<"Create post with id: "<<p.id()<<" author: "<<p.author()<<" likes: "<<p.likes()<<" and message: \""<<p.message()<<"\""<<endl;


    cout<<"---CREATE DIRECT MESSAGE---"<<endl;
    DirectMessage d(1, 2, 36, "Test DM", 14);
    cout<<"Create DM with id: "<<d.id()<<" author: "<<d.author()<<" likes: "<<d.likes()<<" message: \""<<d.message()<<"\""<<" and recipient: "<<d.recipient()<<endl;


    cout<<"---READ POSTS---"<<endl;
    n2.read_posts("C://cygwin64//home//yelir//SocialNetwork//SocialNetwork//posts-1.txt");


    cout << "User #1 has "<< n2.get_user(1)->num_messages() <<" messages"<<endl;

    cout << "Display posts from net:\n" << n2.displayPosts(n2.get_user(1)->name(), 4) << endl; // they have no posts (all DMs)

    cout << "Display DMs from net:\n" << n2.displayDM(n2.get_user(1)->name(), n2.get_user(93)->name(), 2)<< endl; // there really are just duplicates

    cout << "Display posts from net:\n" << n2.displayPosts(n2.get_user(2)->name(), 4) << endl;

    cout<<"---ADD POSTS TO USER---"<<endl;
    int post_counter(296);

    Post p1(++post_counter, 1, 14, "First post");
    Post p2(++post_counter, 1, 15, "Second post");
    Post p3(++post_counter, 1, 16, "Third post");
    DirectMessage d1(++post_counter, 1, 17, "First DM", 53);
    Post p4(++post_counter, 1, 17, "Fourth post");

    Post* posts[] = {&p1, &p2, &p3, &d1, &p4};
    for(auto & p: posts)
        n.get_user(p->author())->addPost(p);

    cout<<"---DISPLAY POSTS FROM USER---"<<endl;

    cout << "Display posts from user:\n"<< n.get_user(p1.author())->displayPosts(3) << endl;

    cout << "Display DMs from user:\n" << n.get_user(d1.author())->displayDMs(d1.recipient(), n.get_user(d1.recipient())->name(), 3) << endl;

    cout<<"---DISPLAY POSTS FROM NET---"<<endl;

    cout << "Display posts from net:\n" << n.displayPosts(n.get_user(p1.author())->name(), 2) << endl;

    cout << "Display DMs from net:\n" << n.displayDM(n.get_user(d1.author())->name(), n.get_user(d1.recipient())->name(), 2)<< endl;

    cout << "Display posts from net (not exist):\n" << n.displayPosts(n.get_user(54)->name(), 2) << endl;

    cout << "Display DMs from net (not exist):\n" << n.displayDM(n.get_user(d1.author())->name(), n.get_user(101)->name(), 2)<< endl;

    cout << "Display DMs from net (not exist):\n" << n.displayDM(n.get_user(54)->name(), n.get_user(63)->name(), 2)<< endl;


    cout<<"---ADD POSTS TO NET---"<<endl;

    n.addPost(n.get_user(2)->name(), "2nd First Post", 18, ++post_counter);
    n.addPost(n.get_user(2)->name(), "2nd Second Post", 19, ++post_counter);
    n.addDM(n.get_user(2)->name(), "2nd First DM", 20, ++post_counter, n.get_user(3)->name());
    n.addDM(n.get_user(2)->name(), "2nd Second DM", 21, ++post_counter, n.get_user(3)->name());
    n.addPost(n.get_user(2)->name(), "2nd Third Post", 12, ++post_counter);

    cout << "Display posts from net:\n" << n.displayPosts(n.get_user(2)->name(), 2) << endl;
    cout << "Display DMs from net:\n" << n.displayDM(n.get_user(2)->name(), n.get_user(3)->name(), 2)<< endl;

}


int main()
{

//    test();
//    return 0;

    Network net;
    net.read_friends("C://cygwin64//home//yelir//SocialNetwork//SocialNetwork//SN-2.txt");
    net.read_posts("C://cygwin64//home//yelir//SocialNetwork//SocialNetwork//posts-1.txt");
    std::size_t id_counter = net.num_users();

    int choice(0), birth, zip, result, id1, id2, howmany;
    std::string name1, name2, first_name, last_name, file_name_string;
    char * file_name;
    User * u;

    while(choice != 13)
    {
        cout<<"\nPick a command:\n"
            <<"1) Add a user [<name> <birth> <zip>] \n"
            <<"2) Add friend connection [<name> <name>] \n"
            <<"3) Remove friend connection [<name> <name>] \n"
            <<"4) Print users [None] \n"
            <<"5) Print friends [<name>] \n"
            <<"6) Write to file [<file_name.txt>] \n"
            <<"7) Shortest Path [<name> <name>] \n"
            <<"8) Disjointed Sets [None] \n"
            <<"9) Get suggested friends [<name>] \n"
            <<"10) Suggest from distance [<name> <distance>] \n"
            <<"11) View recent posts [<name> <amount>] \n"
            <<"12) View recent DMs [<name> <name> <amount>] \n"
            <<"13) End [None] \n";

        cin>>choice;

        if(choice == 1)
        {
            cin>>first_name>>last_name>>birth>>zip;
            u = new User(id_counter++, first_name + " " + last_name, birth, zip, vector<size_t>()); // why this?
            net.add_user(* u);
        }
        else if(choice == 2)
        {
            cin>>first_name>>last_name;
            name1 = first_name + " " + last_name;
            cin>>first_name>>last_name;
            name2 = first_name + " " + last_name;
            result = net.add_connection(name1, name2);
            if(result == -1)
                cout<<"Could not add connection!"<<endl;
        }
        else if(choice == 3)
        {
            cin>>first_name>>last_name;
            name1 = first_name + " " + last_name;
            cin>>first_name>>last_name;
            name2 = first_name + " " + last_name;
            result = net.remove_connection(name1, name2);
            if(result == -1)
                cout<<"Could not remove connection!"<<endl;
        }
        else if(choice == 4)
        {
            cout<<"ID  Name    Year  Zip \n ==========================================================="<<endl;
            net.print();
;        }
        else if(choice == 5)
        {
            cin>>first_name>>last_name;
            cout<<"ID  Name    Year  Zip \n ==========================================================="<<endl;
            result = net.print_friends(first_name + " " + last_name);
            if(result == -1)
                cout<<"Could not print friends!"<<endl;

        }
        else if(choice == 6)
        {
            cin>>file_name_string;
            file_name = &file_name_string[0];
            result = net.write_friends(file_name);
            if(result == -1)
                cout<<"Could write!"<<endl;
            else{
                cout<<"Wrote "<<net.num_users()<<" users to file."<<endl;
            }
        }
        else if(choice == 7)
        {
            cin>>first_name>>last_name;
            name1 = first_name + " " + last_name;
            cin>>first_name>>last_name;
            name2 = first_name + " " + last_name;
            id1 = net.get_id(name1);
            id2 = net.get_id(name2);
            if(id1 == -1 || id2 == -1)
                cout<<"Could not find users!"<<endl;
            else
            {
                std::vector<int> path_result = net.shortest_path(id1, id2);
                if(path_result.empty())
                    cout<<"No path exists!"<<endl;
                else
                {
                    cout<<"Distance: "<<net.get_user(path_result[path_result.size()-1])->depth<<endl;
                    for(std::size_t i = 0; i < path_result.size() - 1; ++i)
                        cout<<net.get_user(path_result[i])->name()<<" -> ";
                    cout<<net.get_user(path_result[path_result.size()-1])->name()<<endl;
                }
            }
        }
        else if(choice == 8)
        {
            std::vector<vector<int>> groups_result = net.groups();
            for(std::size_t i = 0; i < groups_result.size(); ++i)
            {
                cout<<"Set "<<i+1<<" => ";
                for(auto j: groups_result[i])
                    cout<<net.get_user(j)->name()<<", ";
                cout<<endl;
            }
        }
        else if(choice == 9)
        {
            cin>>first_name>>last_name;
            name1 = first_name + " " + last_name;
            int score;
            std::vector<int> suggest_result = net.suggest_friends(net.get_id(name1), score);
            if(score == -1)
                cout<<"None"<<endl;
            else
            {
                cout<<"The suggested friend(s) is/are:"<<endl;
                for(auto e: suggest_result)
                {
                    cout<<net.get_user(e)->name()<<" with score "<<score<<endl;
                }
            }
        }
        else if(choice == 10)
        {
            int to, distance;
            cin>>first_name>>last_name>>distance;
            name1 = first_name + " " + last_name;
            std::vector<int> distance_path = net.distance_user(net.get_id(name1), to, distance);
            if(to == -1)
                cout<<"No match was calculated"<<endl;
            else
            {
                cout<<"Found a match! "<<net.get_user(to)->name()<<": ";
                for(std::size_t i = 0; i < distance_path.size() - 1; ++i)
                    cout<<net.get_user(distance_path[i])->name()<<" -> ";
                cout<<net.get_user(distance_path[distance_path.size()-1])->name()<<endl;
                cout<<endl;
            }
        }
        else if(choice == 11)
        {
            cin>>first_name>>last_name>>howmany;
            name1 = first_name + " " + last_name;
            std::string post_result = net.displayPosts(name1, howmany);
            if(post_result == "")
                cout << "No posts" << endl;
            else
                cout << "Displaying posts:\n===========================================================\n" << post_result<< endl;
        }
        else if(choice == 12)
        {
            cin>>first_name>>last_name;
            name1 = first_name + " " + last_name;
            cin>>first_name>>last_name;
            name2 = first_name + " " + last_name;
            cin>>howmany;
            std::string DM_result = net.displayDM(name1, name2, howmany);
            if(DM_result == "")
                cout << "No DMs" << endl;
            else
                cout << "Displaying DMs:\n===========================================================\n" << DM_result << endl;
        }
        else
        {
            cout<<"Program end."<<endl;
            choice = 13;
            break;
        }
    }

    return 0;
}
