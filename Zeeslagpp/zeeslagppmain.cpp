#include <iostream>
#include <ships.h>
#include <string.h>
#include <zmq.hpp>
#include <unistd.h>

using namespace std;

int main()
{
    string subscribestring ="<zeeslag><username><";
    string total;
    string username;
    string rcvbuf;
    string user;
    vector <string> players(0);

    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);


    int first= 0;
    int last = 0;

//connect publisher en subscriber to benternet
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    // publisher en subscriber connected or not ?
    while(1)
    {
        total ="\0";
        username ="\0";
        rcvbuf ="\0";
        user ="\0";
        zmq::message_t msg;
        //msg.empty();

    //without user application ask for username
        cout << "username:";
        cin >> username;
        cout<<endl;

        total.append(subscribestring);
        total.append(username);
        total.append(">");
        cout<<total<<"\n"<<endl;

    // socketopt voor subscribe on zeeslag
        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><username>");

        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout <<"ask send"<<endl;
        //std::cout << "searching for players"<< std::endl;

    //subscriber true
        subscriber.recv(msg);
        rcvbuf=(char *)(msg.data());
        cout<<"message received:"<< rcvbuf<<"\n";
        //cout <<"length="<< rcvbuf.length()<<" size="<< rcvbuf.size()<<endl;

        first =total.find_last_of("<")+1;
        last = total.find_last_of(">");

        user=total.substr(first,(last-first));
        //cout<<first <<"\n"<< last<<"\n"<<user<<"\n"<<endl;

    //user in vector
        players.push_back(user);
        //print players list + location.
        cout <<"user in list"<<endl;
        for(int a =0;a<int(players.size());a++)
        {
            cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\t"<<"length:"<< string(players.at(a)).length()<<"\n";
        }

        //memset(&username,'\0',username.size());
    }

    subscriber.close();
    publisher.close();
    context.shutdown();


//    ships Carrier(5);
//    ships Battleship(4);
//    ships Cruiser(3);
//    ships Submarine(3);
//    ships Destroyer(2);

//    cout<<"lengte Carrier =\t"<< Carrier.getlenght()<<endl;
//    cout<<"lengte Battleship =\t"<< Battleship.getlenght()<<endl;
//    cout<<"lengte Cruiser =\t"<< Cruiser.getlenght()<<endl;
//    cout<<"lengte Submarine =\t"<< Submarine.getlenght()<<endl;
//    cout<<"lengte Destroyer =\t"<< Destroyer.getlenght()<<endl;

    return 0;
}
