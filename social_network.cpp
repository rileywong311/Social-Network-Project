#include <iostream>
#include "network.h"
#include <sstream>
#include <cassert>
using namespace std;

void test()
{
    Network n;

    User u1(0, "Dev Pathak", 2003, 50505, vector<size_t>());
    User u2(1, "Rushil Patel", 1999, 11111, vector<size_t>());
    User u3(2, "Austin Min", 1472, 50505, vector<size_t>());
    User u4(3, "Guy Fieri", 1642, 31121, vector<size_t>());
    User u5(21, "David Hasselhoff", 420, 69420, vector<size_t>());

    User* list[] = {&u1, &u2, &u3, &u4, &u5};
    for(auto & e: list)
        n.add_user(*e);

    n.add_connection(u1.name(), u2.name());
    n.add_connection(u2.name(), u3.name());
    n.add_connection(u2.name(), u4.name());
    n.add_connection(u4.name(), u5.name());
    cout << "try duplicate: " << n.add_connection(u2.name(), u4.name()) << endl; // try duplicate (works, but User method "add_connection" handles the duplication)
    cout << "name does not exist: " << n.add_connection(u2.name(), "Not exist") << endl << endl; // name does not exist, so errors with -1
    n.write_friends("write.txt");

    n.remove_connection(u1.name(), u2.name());
    n.remove_connection(u1.name(), u4.name()); // link does not exist
    cout << "name does not exist: " << n.remove_connection(u1.name(), "Not exist") << endl << endl; // name does not exist, so errors with -1

    n.write_friends("write2.txt");

    Network n2;
    n2.read_friends("write2.txt");
    n2.write_friends("write3.txt");
}


int main()
{
    std::size_t id_counter = 0;
    Network net;

    int choice(0), birth, zip, result;
    std::string name1, name2, first_name, last_name, file_name_string;
    char * file_name;
    User * u;

    while(choice != 7)
    {
        cout<<"\nPick a command:\n"
            <<"1) Add a user [<name> <birth> <zip>] \n"
            <<"2) Add friend connection [<name> <name>] \n"
            <<"3) Remove friend connection [<name> <name>] \n"
            <<"4) Print users [None] \n"
            <<"5) Print friends [<name>] \n"
            <<"6) Write to file [<file_name.txt>] \n"
            <<"7) End [None]] \n";

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
        else
        {
            cout<<"Program end."<<endl;
            choice = 7;
        }
    }

    return 0;
}
