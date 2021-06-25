#include <iostream>
#include <zmq.hpp>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

int main()
{
    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);

    string pubstring ="<zeeslag><username><";
    string substring ="<zeeslag><";
    string total;
    string substringtot;
    string username;
    string opponent;
    string rcvbuf;
    string coord;
    string opscore;
    int last;
    int first;
    int score;
    int newscore;
    string hits;
    string attack;
    string recvcor;
    string sendcor;

//connect publisher en subscriber to benternet
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    // publisher en subscriber connected or not ?
    bool status = true;
    bool program = true;
    cout << "username:";
    cin >> username;
    cout<<endl;
    cout<<"score:";
    cin>>score;
    cout<<endl;
    while(program)
    {

        status = true;
        zmq::message_t msg;
        zmq::message_t rcv;


        cout <<username<<" your startsscore ="<<score<<endl;

    // compose a message to send "<zeeslag><username><"+ username + >
        hits = to_string(score);
        total.append(pubstring);
        total.append(username);
        total.append(">");
        total.append("<");
        total.append(hits);
        total.append(">");

        publisher.send(zmq::buffer(total,total.size()),zmq::send_flags::none);//"<zeeslag><username><+"username"+>                                                   //send composed message
        zmq::buffer("\0");

        cout<<"publisher.send:"<<total<<endl;
        //cout<<"message send:"<<username<<username.size()<<"\t"<<username.length()<<endl;
        total.clear();

    // compose a message to subscribe on "<zeeslag>< + username + >"
        substringtot.append(substring);
        substringtot.append(username);
        substringtot.append(">");

        subscriber.set(zmq::sockopt::subscribe,substringtot);
        sleep(2);
        cout<<"subscribing on"<<substringtot<<endl;

    //  receiving a message + output wotdt geparsed
        subscriber.recv(msg,zmq::recv_flags::none);                                                                                                     // recv message to append
        rcvbuf=(char *)(msg.data());
        cout<<"received message:"<<rcvbuf<<endl; //received message:<zeeslag><tom><your opponent is bart><0>

        //first =rcvbuf.find_last_of("<")+1;

        first=rcvbuf.find_last_of(" ")+1;
        last= rcvbuf.find_first_of(">",first);
        opponent = rcvbuf.substr(first,last-first);

        int firstscore = rcvbuf.find_last_of("<")+1;
        int lastscore = rcvbuf.find_first_of(">",firstscore);
        opscore=rcvbuf.substr(firstscore,(lastscore-firstscore));
        cout<<opscore<<endl;
        int scored = stoi(opscore);
        if(scored<=score)
        {
            cout<<username<<" u may begin"<<endl;
        }
        else
        {
            cout<<username<<"u may not begin"<<endl;
        }


        subscriber.set(zmq::sockopt::unsubscribe,substringtot);

        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+opponent+">");//+"<"+username+">");
            //<zeeslag><opponent><tom>
        cout<<"\nGive the target coordinates between a1 en i9"<<endl;
        cout<<"the letters from a to i represent the colums."<<endl;
        cout<<"the numbers from 1 to 9 represents the rows\n"<<endl;
        cout<<"to start a new game typ exit"<<endl;
        cout<<"to shutdown the program typ shutdown"<<endl;

        while(status)
        {
            cout<<"coordinates to attack:";
            cin >> coord;
            while(coord.size()>2&&coord != "exit"&&coord!= "shutdown")
            {
                coord.clear();
                cout<<"Missile out of range"<<endl;
                cout<<username<<":";
                cin >>coord;
            }

            sendcor.append(substringtot+"<"+opponent+">"+"<"+coord+">");
            publisher.send(zmq::buffer(sendcor,sendcor.size()),zmq::send_flags::none);                                                                             //send composed message

            sendcor.clear();
            if(coord == "exit")
            {
                cout<<"exiting the game"<<endl;
                cout<<"what is your score:";
                cin>>newscore;
                score = score+newscore;
                status = false;
            }
            else if(coord =="shutdown")
            {
                cout<<"exiting program"<<endl;
                program = false;
                status = false;
            }
            else
            {
                cout<<"\nshooting\n"<<endl;
                subscriber.recv(msg,zmq::recv_flags::none);

                recvcor = (char*)(msg.data());                                                                                                      //recv message to append
                attack =recvcor.substr((recvcor.find_last_of("<")+1),(recvcor.find_last_of(">")-1)-(recvcor.find_last_of("<")));
                if (attack == "exit"|| attack == "shutdown")
                {
                    cout<<opponent<<" opponent left the game"<<endl;

                }
                else
                {
                cout<<opponent<<" attacked:"<<attack<<endl;
                }
                attack.clear();
            }

        }
    substringtot.clear();
    subscriber.set(zmq::sockopt::unsubscribe,"<zeeslag><"+opponent+">");

    }
    cout<<"Goodbye"<<endl;
    publisher.close();
    subscriber.close();
    context.shutdown();

    return 0;
}
