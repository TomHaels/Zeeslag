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
    string tosend;
    string rcvchoice;
    int random;
    int select;
    string choices ="choose a number:\n1: select a player\n2: Random player\n3: add a player";
    int choice;
    vector <string> players(0);
    srand (time(NULL));


    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);


    int first= 0;
    int last = 0;

//connect publisher en subscriber to benternet
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

    // publisher en subscriber connected or not ?

    subscriber.set(zmq::sockopt::subscribe,"<zeeslag><username>");

    cout<<"starting service"<<endl;
    sleep(2);
    cout<<"service running"<<endl;

    while(1) // running program
    {
    //clear the strings
        total ="\0";
        username ="\0";
        rcvbuf ="\0";
        user ="\0";


        zmq::message_t msg;
        zmq::message_t recvchoice;
        zmq::message_t choicemade;

    //subscriber true message received
        cout<<"searching for users"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);
        cout<<"users found"<<endl;

    //msg in rcvbuf + append the username
        rcvbuf=(char *)(msg.data());
        first =rcvbuf.find_last_of("<")+1;
        last = rcvbuf.find_first_of(">",first);

        user=rcvbuf.substr(first,(last-first));
        cout<<user<<endl;
        rcvbuf.resize(subzeeslag.size()+subuser.size()+user.size()+2);
        cout<<"message received:"<< rcvbuf<<"\n";
        //cout <<"length="<< rcvbuf.length()<<" size="<< rcvbuf.size()<<endl;
        //cout<<first <<"\n"<< last<<"\n"<<user<<"\n"<<endl;

    //add user in vector
        players.push_back(user);
        //print players list + location.
        cout <<"user in list"<<endl;
        for(int a =0;a<int(players.size());a++)
        {
            cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\n";
        }

    //append <zeeslag>+<user> for communication
        talktouser.append(subzeeslag+"<"+user+">");
        subscriber.set(zmq::sockopt::subscribe,talktouser);
        cout<<"waiting to talk"<<endl;
        sleep(2);
        cout<<"ready"<<endl;


        tosend.append(talktouser+choices);
        //cout<<"choice:";
        //cin >>choice;

        //cout<<"talk to user:"<<tosend<<endl;

            publisher.send(zmq::buffer(tosend),zmq::send_flags::none);
            tosend.clear();
            cout<<"send"<<endl;
            subscriber.recv(choicemade,zmq::recv_flags::none);
            cout<<"recv"<<endl;
            rcvchoice=(char *)(choicemade.data());
            cout<<"rcvchoice:"<<rcvchoice<<"end"<<endl;
            //choice = stoi(rcvchoice);
            //cout<<choice<<endl;
            // to be continued =


//                switch(choice)
//                {

//                    case '1':
//                        cout<<"choose a number\n"<<endl;
//                        for(int a =0;a<int(players.size());a++)
//                        {
//                            cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\t"<<"length:"<< string(players.at(a)).length()<<"\n";
//                        }
//                        cout<<"choose a number"<<endl;
//                        cin>>select;
//                        cout<<"you chose player:"<<string(players.at(select-1))<<"\n"<<endl;

//                    break;
//                    case '2':
//                      /* generate secret number between 1 and 10: */
//                      random = rand() % int(players.size()) + 1;
//                      cout<<"random chose:"<<string(players.at(random-1))<<" "<<random<<endl;

//                    break;

//                    case '3':
//                    cout<<"name:";
//                    cin>>user;
//                    cout<<endl;

//                    players.push_back(user);
//                    cout<<"player added"<<endl;

//                    break;
//                default:
//                    cout<<"another input"<<endl;

//                }


    }

    subscriber.close();
    publisher.close();
    context.shutdown();




    return 0;
}
