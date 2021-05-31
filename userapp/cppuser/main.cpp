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
    string username="emptyval";
    string opponent;
    string rcvbuf;
    string user;
    int last;
    int first;
    string colum;
    int letter;
    string recvcor;

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
        zmq::message_t rcv;
        //msg.empty();

    //without user application ask for username
        cout << "username:";
        cin >> username;
        cout<<endl;

        total.append(pubstring);
        total.append(username);
        total.append(">");

        publisher.send(zmq::buffer(total),zmq::send_flags::none);//"<zeeslag><username><+"username"+>
        cout<<"message send:"<<username<<username.size()<<"\t"<<username.length()<<endl;
        //total.clear();

        substringtot.append(substring);
        substringtot.append(username);
        substringtot.append(">");
        subscriber.set(zmq::sockopt::subscribe,substringtot);//"<zeeslag><" + "username" + ">"
        sleep(2);
        cout<<"subscribing on"<<substringtot<<endl;


        subscriber.recv(msg,zmq::recv_flags::none);
        rcvbuf=(char *)(msg.data());

        cout<<"rcvmessage:"<<rcvbuf<<endl;
        first=rcvbuf.find_last_of(" ")+1;
        last= rcvbuf.find_last_of(">");
        opponent = rcvbuf.substr(first,last-first);

        subscriber.set(zmq::sockopt::unsubscribe,substringtot);
        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+opponent+">"+"<"+username+">");
        //cout<<"subscribed on:<zeeslag><"<<opponent<<">"<<"<"+username+">"<<endl;
        cout<<"\nGive the target coordinates between a1 en i9"<<endl;
        cout<<"the letters from a to i represent the colums."<<endl;
        cout<<"the numbers represents the rows"<<endl;
        while(1)
        {
        cout<<"colum:";
        cin >> colum;
        cout<<"\nrow:";
        cin>>letter;
        cout<<endl;


        publisher.send(zmq::buffer(substringtot+"<"+opponent+">"+colum),zmq::send_flags::none);
        cout<<"send"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);
        cout<<"rcv"<<endl;
        recvcor = (char*)(rcv.data());
        cout<<"received:"<<recvcor<<endl;
        recvcor.clear();
        }
//        rcvbuf.clear();
//        sendchoice = to_string(choice);
//        cout<<"string:"<<sendchoice<<"int:"<<(int)choice<<endl;
//        publisher.send(zmq::buffer(sendchoice),zmq::send_flags::none);



    }
    return 0;
}
