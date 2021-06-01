#include <iostream>
#include <thread>
#include <list>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>

using namespace std;
zmq::context_t context(1);
string user;
string tegenstander;
string men;
int a = 0;
void cor()
{
    zmq::message_t msg;
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);
    string buffer;
    string playero = men;
    string playert = tegenstander;
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    for(;;)
    {
        if (a == 1)
        {

        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+playero+"><"+playert+">");
        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+playert+"><"+playero+">");
        sleep(2);
        cout<<"ready to recv coord"<<endl;
        cout<<user<<endl;
        cout<<tegenstander<<endl;
            for(;;)
            {
                subscriber.recv(msg,zmq::recv_flags::none);
                buffer=(char*)(msg.data());
                cout<<"coor:"<<buffer<<endl;
                buffer.clear();
            }
        }
    }


}
void adduser()
{
    int first =0;
    int last = 0;

    string rcvbuf;
    string subzeeslag ="<zeeslag>";
    string subuser = "<username>";

    zmq::message_t msg;
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);

    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

    subscriber.set(zmq::sockopt::subscribe,"<zeeslag><username>");
    sleep(2);//add a delay so there is enough time subcribing is performed

    cout<<"service started.."<<endl;
    cout<<"subscribing on <zeeslag><username>"<<endl;

    for(;;)
    {
        cout<<"searching for players"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);
        rcvbuf=(char *)(msg.data());
        cout<<"subscriber.recv:"<<rcvbuf<<endl;

    //get user out of recv string PARSING
        first =rcvbuf.find_last_of("<")+1;
        last = rcvbuf.find_first_of(">",first);

        user=rcvbuf.substr(first,(last-first));
        cout<<"player:";
        cout<<user<<endl;
        cout<<endl;
        rcvbuf.resize(subzeeslag.size()+subuser.size()+user.size()+2);
    }

}

void printList()
{
    vector <string> players(0);
    string subzeeslag ="<zeeslag>";
    string subuser = "<username>";
    string talktouser;
    string opponent;
    string playerone;
    string playertwo;
    string playname;
    string recvcor;

    zmq::message_t msg;
    zmq::message_t rcv;
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);

    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

    for(;;)
    {
        if (user.size()>0)
        {
        //add user in vector
            players.push_back(user);
            playname = user;
            men = user;
            cout <<"user added"<<endl;
            for(int a =0;a<int(players.size());a++)
            {
                cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<endl;
            }
            user.clear();

        //append <zeeslag>+<user> for communication
                talktouser.append("<zeeslag><"+playname+">");

                //subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+playname+">"); //subscribe one <zeeslag><user>
                //cout<<"\nsubscribing on "<<"<zeeslag><"<<playname<<">"<<endl;
                //sleep(2);
                //cout<<"subscribing\n"<<endl;

        //---------select player in vector----------------
                if(players.size()>1)
                {
                    opponent= string(players.at(0)); //longest waiting player
            //
                    playerone.append(subzeeslag+"<"+playname+">"+"<your opponent is "+opponent+">");
                    playertwo.append(subzeeslag+"<"+opponent+">"+"<your opponent is "+playname+">");
                    cout<<playname<<" is playing against "<<opponent<<endl;
                    tegenstander = opponent;
            //Playing players out of choosing list

                    cout <<"\nuser deleted"<<endl;
                    players.erase(players.end());
                    players.erase(players.begin());
                    for(int a =0;a<int(players.size());a++)
                    {
                        cout << int(players.size())-(int(players.size())-a)+1 <<endl;//to check if vector is empty
                    }
                    //zmq::buffer("\0");
                    publisher.send(zmq::buffer(playerone),zmq::send_flags::none);
                    cout<<"publisher.send"<<playerone<<endl;
                    publisher.send(zmq::buffer(playertwo),zmq::send_flags::none);
                    cout<<"publisher.send"<<playertwo<<endl;
                    playerone.clear();
                    playertwo.clear();


                    //cout<<"subscribed on:"<<"<zeeslag><"<<playname<<">"<<"<"<<opponent<<">"<<endl;
                    //sleep(2);
                    cout<<men<<endl;
                    cout<<tegenstander<<endl;
                    a = 1;
                    //recvcor.clear();
                    //subscriber.recv(rcv,zmq::recv_flags::none);

                    //recvcor = (char*)(rcv.data());
                    //cout<<"received:"<<recvcor<<endl;


                    }
                }


        }
    }


int main()
{
    while(1)
    {

    thread t1(adduser);
    thread t2(printList);
    thread t3(cor);

    t1.join();
    t2.join();
    t3.join();


    }
    return 0;
}
