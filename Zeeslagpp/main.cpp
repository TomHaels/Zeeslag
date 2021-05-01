#include <iostream>
#include <ships.h>
#include <strings.h>
#include <zmq.hpp>


using namespace std;

int main()
{


    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::push);

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:5555");

    // set up some static data to send
    const std::string data{"Hello"};

    for (auto request_num = 0; request_num < 10; ++request_num)
    {
        // send the request message
        std::cout << "Sending Hello " << request_num << "..." << std::endl;
        socket.send(zmq::buffer(data), zmq::send_flags::none);

        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string();
        std::cout << " (" << request_num << ")";
        std::cout << std::endl;
    }

    ships Carrier(5);
    ships Battleship(4);
    ships Cruiser(3);
    ships Submarine(3);
    ships Destroyer(2);

    cout<<"lengte Carrier =\t"<< Carrier.getlenght()<<endl;
    cout<<"lengte Battleship =\t"<< Battleship.getlenght()<<endl;
    cout<<"lengte Cruiser =\t"<< Cruiser.getlenght()<<endl;
    cout<<"lengte Submarine =\t"<< Submarine.getlenght()<<endl;
    cout<<"lengte Destroyer =\t"<< Destroyer.getlenght()<<endl;

    return 0;
}
