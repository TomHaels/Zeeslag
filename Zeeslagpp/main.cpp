#include <iostream>
#include <ships.h>
#include <string.h>
#include <zmq.hpp>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;
int main()
{
    string subzeeslag ="<zeeslag>";
    string subuser = "<username>";
    string total;
    string username;
    string rcvbuf;
    string user;
    string talktouser;
    vector <string> players(0);

    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);
    zmq::socket_t talker(context,ZMQ_SUB);

    int first= 0;
    int last = 0;

//connect publisher en subscriber to benternet
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    talker.connect("tcp://benternet.pxl-ea-ict.be:24042");

    // publisher en subscriber connected or not ?

    subscriber.set(zmq::sockopt::subscribe,"<zeeslag><username>");

    cout<<"starting service"<<endl;
    sleep(2);
    cout<<"service running"<<endl;

    while(1)
    {
    //clear the strings
        total ="\0";
        username ="\0";
        //rcvbuf ="\0";
        user ="\0";


        zmq::message_t msg;

    //without user application ask for username
        cout << "username:";
        cin >> username;
        cout<<endl;

        total.append(subzeeslag+subuser+"<"+username+">");
        total.append(subuser);
        total.append("<");
        total.append(username);
        total.append(">");

      cout<<total<<"\n"<<endl;

    // socketopt voor subscribe on zeeslag

        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout <<"ask send:"<<total<<endl;
        //std::cout << "searching for players"<< std::endl;

    //subscriber true
        cout<<"searching for users"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);
        cout<<"users found"<<endl;
        rcvbuf=(char *)(msg.data());
        cout<<rcvbuf<<endl;
        first =total.find_last_of("<")+1;
        last = total.find_last_of(">");

        user=total.substr(first,(last-first));
        cout<<user<<endl;
        rcvbuf.resize(subzeeslag.size()+subuser.size()+user.size()+2);
        cout<<"message received:"<< rcvbuf<<"\n";
        cout <<"length="<< rcvbuf.length()<<" size="<< rcvbuf.size()<<endl;
        //cout<<first <<"\n"<< last<<"\n"<<user<<"\n"<<endl;

    //user in vector
        players.push_back(user);
        //print players list + location.
        cout <<"user in list"<<endl;
        for(int a =0;a<int(players.size());a++)
        {
            cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\t"<<"length:"<< string(players.at(a)).length()<<"\n";
        }

//        talktouser.append(subzeeslag+"<"+user+">");
//        talker.set(zmq::sockopt::subscribe,talktouser);
//        cout<<"waiting to talk"<<endl;
//        sleep(2);
//        cout<<"ready"<<endl;

//        string tosend;
//        string bericht;
//        cout<<"message:";
//        cin >>bericht;
//        tosend.append(talktouser+bericht);
//        cout<<"talk to user:"<<tosend<<endl;
//        publisher.send(zmq::buffer(tosend),zmq::send_flags::none);
    }

    subscriber.close();
    publisher.close();
    context.shutdown();




    return 0;
}
