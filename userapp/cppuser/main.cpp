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

        total.append(pubstring);
        total.append(username);
        total.append(">");

        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout<<"message send:"<<total<<total.size()<<"\t"<<total.length()<<endl;

        substringtot.append(substring);
        substringtot.append(username);
        substringtot.append(">");
        cout<<"ready to recv on "<<substringtot<<endl;

//        subscriber.set(zmq::sockopt::subscribe,substringtot);
//        sleep(5);

//        subscriber.recv(msg,zmq::recv_flags::none);
//        rcvbuf=(char *)(msg.data());
//        cout<<"rcvmessage:"<<rcvbuf<<endl;

    }
    return 0;
}
