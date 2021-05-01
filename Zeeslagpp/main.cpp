#include <iostream>
#include <ships.h>
#include <strings.h>
#include <zmq.hpp>
#include <zmq_addon.hpp>


using namespace std;

int main()
{


    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::push);



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
