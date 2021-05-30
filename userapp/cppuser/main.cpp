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
    string rcvbuf;
    string user;
    int choice;
    string sendchoice;
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

        total.append(pubstring);
        total.append(username);
        total.append(">");

        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout<<"message send:"<<total<<total.size()<<"\t"<<total.length()<<endl;
        total.clear();

        substringtot.append(substring);
        substringtot.append(username);
        substringtot.append(">");

        subscriber.set(zmq::sockopt::subscribe,substringtot);
        sleep(2);


            subscriber.recv(msg,zmq::recv_flags::none);
            rcvbuf=(char *)(msg.data());
            cout<<"rcvmessage:"<<rcvbuf<<endl;
            cout<<"choice:";
            cin>>choice;
            cout<<endl;
            rcvbuf.clear();
            sendchoice = to_string(choice);
            cout<<"string:"<<sendchoice<<"int:"<<(int)choice<<endl;
            publisher.send(zmq::buffer(sendchoice),zmq::send_flags::none);


    }
    return 0;
}
