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
    zmq::context_t contextt(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);
    zmq::socket_t talker(contextt,ZMQ_SUB);

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
        rcvbuf ="\0";
        user ="\0";


        zmq::message_t msg;

    //subscriber true
        cout<<"searching for users"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);
        cout<<"users found"<<endl;

        rcvbuf=(char *)(msg.data());
        first =rcvbuf.find_last_of("<")+1;
        last = rcvbuf.find_first_of(">",first);

        user=rcvbuf.substr(first,(last-first));
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

        talktouser.append(subzeeslag+"<"+user+">");
        talker.set(zmq::sockopt::subscribe,talktouser);
        cout<<"waiting to talk"<<endl;
        sleep(2);
        cout<<"ready"<<endl;

        string tosend;
        string bericht;
        cout<<"message to"<<talktouser<<":";
        cin >>bericht;
        tosend.append(talktouser+bericht);
        cout<<"talk to user:"<<tosend<<endl;
        publisher.send(zmq::buffer(tosend),zmq::send_flags::none);


        // to be continued
        vector <string> players({"tom","obi","jessica","pushkin"});
        string user;
        int choice;
        int select;
        int random;
        srand (time(NULL));
    choice = 0;
        while(1)
        {

            switch(choice)
            {
                case 0:
                cout<<"choose:\n1: select a player\n2: Random player\n3: add a player"<<endl;
                cin>>choice;
                break;

                case 1:
                    cout<<"choose a number\n"<<endl;
                    for(int a =0;a<int(players.size());a++)
                    {
                        cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\t"<<"length:"<< string(players.at(a)).length()<<"\n";
                    }
                    cout<<"choose a number"<<endl;
                    cin>>select;
                    cout<<"you chose player:"<<string(players.at(select-1))<<"\n"<<endl;
                    choice =0;
                break;
                case 2:
                  /* generate secret number between 1 and 10: */
                  random = rand() % int(players.size()) + 1;
                  cout<<"random chose:"<<string(players.at(random-1))<<" "<<random<<endl;
                  choice =0;
                break;

                case 3:
                cout<<"name:";
                cin>>user;
                cout<<endl;

                players.push_back(user);
                cout<<"player added"<<endl;
                choice =0;
                break;
            default:
                cout<<"another input"<<endl;

            }
        }
    }

    subscriber.close();
    publisher.close();
    context.shutdown();




    return 0;
}
