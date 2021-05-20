#include <iostream>
#include <ships.h>
#include <string.h>
#include <zmq.hpp>
#include <unistd.h>

using namespace std;

int main()
{

    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);
    string rcvbuf;
    int flag=0;
    zmq::message_t msg;

    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    const string total ="hallo";
    const string zeeslag ="zeelsag";
    string buf;

    // set up some static data to send
    //const std::string data{"Hello"};
        subscriber.set(zmq::sockopt::subscribe,"zeeslag");

        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout <<"ask send"<<endl;
        std::cout << "searching for players"<< std::endl;
        flag =1;
        auto rc= subscriber.recv(msg,zmq::recv_flags::none);
        rcvbuf=(char *)(msg.data());


        cout<<"message received "<< rcvbuf<<"\n";

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
