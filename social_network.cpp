#include <iostream>
#include "network.h"
#include <sstream>
#include <cassert>
using namespace std;

void test()
{
    Network n;

    User u0(0, "Dev Pathak", 2003, 50505, vector<size_t>());
    User u1(1, "Rushil Patel", 1999, 11111, vector<size_t>());
    User u2(2, "Austin Min", 1472, 50505, vector<size_t>());
    User u3(3, "Guy Fieri", 1642, 31121, vector<size_t>());
    User u4(4, "David Hasselhoff", 420, 69420, vector<size_t>());
    User u5(5, "Douglas Bowser", 777, 53023, vector<size_t>());
    User u6(6, "Kiryu Kazuma", 1978, 45678, vector<size_t>());

    User* list[] = {&u0, &u1, &u2, &u3, &u4, &u5, &u6};
    for(auto & e: list)
        n.add_user(*e);

    n.add_connection(u0.name(), u1.name());
    n.add_connection(u0.name(), u2.name());
    n.add_connection(u0.name(), u3.name());

    n.add_connection(u1.name(), u3.name());
    n.add_connection(u1.name(), u4.name());

    n.add_connection(u2.name(), u3.name());

    n.add_connection(u3.name(), u6.name());
    n.add_connection(u3.name(), u4.name());

    n.add_connection(u4.name(), u5.name());


    cout<<"---TEST SHORTEST PATH---"<<endl;
    int from(0), to(5);
    std::vector<int> i = n.shortest_path(from, to);
    cout<<"VECTOR SIZE: "<<i.size()<<endl;
    cout<<"DEPTH (FROM): "<<n.get_user(from)->depth<<endl;
    cout<<"DEPTH (TO): "<<n.get_user(i[i.size()-1])->depth<<endl;

    cout<<"Distance: "<<n.get_user(i[i.size()-1])->depth<<endl;
    for(std::size_t j = 0; j < i.size() - 1; ++j)
        cout<<n.get_user(i[j])->name()<<" -> ";
    cout<<n.get_user(i[i.size()-1])->name()<<endl;


    User u7(7, "Bob Builder", 684, 12414, vector<size_t>());
    User u8(8, "Teddy Roosevelt", 1985, 12526, vector<size_t>());
    User u9(9, "Macho Man", 135, 36358, vector<size_t>());

    User* list2[] = {&u7, &u8, &u9};
    for(auto & e: list2)
        n.add_user(*e);

    n.add_connection(u7.name(), u8.name());


    cout<<"\n---TEST DISJOINT SETS---"<<endl;

    std::vector<vector<int>> result = n.groups();
    int counter = 1;
    for(auto i: result)
    {
        cout<<"Group "<<counter++<<endl;
        for(auto j: i)
            cout<<j<<' ';
        cout<<endl;
    }


    for(std::size_t i = 0; i < result.size(); ++i)
    {
        cout<<"Set "<<i+1<<" => ";
        for(auto j: result[i])
            cout<<n.get_user(j)->name()<<", ";
        cout<<endl;
    }

}


int main()
{
//    test();
//    return 0;

    std::size_t id_counter = 0;
    Network net;

    int choice(0), birth, zip, result, id1, id2;
    std::string name1, name2, first_name, last_name, file_name_string;
    char * file_name;
    User * u;

    while(choice != 9)
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
            <<"9) End [None] \n";

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
                std::vector<int> result = net.shortest_path(id1, id2);
                if(result.empty())
                    cout<<"No path exists!"<<endl;
                else
                {
                    cout<<"Distance: "<<net.get_user(result[result.size()-1])->depth<<endl;
                    for(std::size_t i = 0; i < result.size() - 1; ++i)
                        cout<<net.get_user(result[i])->name()<<" -> ";
                    cout<<net.get_user(result[result.size()-1])->name()<<endl;
                }
            }
        }
        else if(choice == 8)
        {
            std::vector<vector<int>> result = net.groups();
            for(std::size_t i = 0; i < result.size(); ++i)
            {
                cout<<"Set "<<i+1<<" => ";
                for(auto j: result[i])
                    cout<<net.get_user(j)->name()<<", ";
                cout<<endl;
            }
        }
        else
        {
            cout<<"Program end."<<endl;
            choice = 9;
            break;
        }
    }

    return 0;
}
