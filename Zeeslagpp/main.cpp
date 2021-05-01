#include <iostream>
#include <ships.h>
#include <string.h>
#include <zmq.hpp>
#include <unistd.h>

using namespace std;

int main()
{


    zmq::context_t context;
    zmq::socket_t publisher(context,zmq::socket_type::pub);
    zmq::socket_t subscriber(context,zmq::socket_type::sub);

    int rc;
    int flag=0;
    zmq::message_t msg;

    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    const string total ="hello";
    string buf;

    // set up some static data to send
    const std::string data{"Hello"};

    for (int i=0; i<5;i++)
    {
        subscriber.setsockopt(ZMQ_SUB,&total,0);
        publisher.send(zmq::buffer(total),zmq::send_flags::none);
        cout <<"ask send"<<endl;
        std::cout << "searching for players"<< std::endl;
        flag =1;

        while(flag ==1)
        {
            subscriber.recv(zmq::buffer(buf),zmq::recv_flags::none);
            cout<<"."<<endl;
            cout<<"."<<endl;
            usleep(1000);

            if ( sizeof(buf)> 0)
            {
                cout <<"\nplay"<<endl;
                flag = 0;

            }
        }

        subscriber.close();
        publisher.close();
        context.shutdown();



    }

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
