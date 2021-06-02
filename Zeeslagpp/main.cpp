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
bool status = 0;
void cor()
{
    zmq::message_t msg;
    zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);
    string buffer;
    string playero;
    string playert;
    string coord;
    string totalsend;
    string exituser;
    //string coordstatus;
    int first;
    int last;
    int exfirstuser;
    int exsecuser;
    publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");                                                                                                       //2.connection benternet
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    for(;;)
    {
        while (status)
        {
        playero= men;
        playert = tegenstander;
        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+playero+"><"+playert+">");
        subscriber.set(zmq::sockopt::subscribe,"<zeeslag><"+playert+"><"+playero+">");
        sleep(2);
        cout<<"ready to recv coord"<<endl;

            while(status)
            {
                subscriber.recv(msg,zmq::recv_flags::none);                                                                                                         //5+6.1st recv + parsing
                buffer=(char*)(msg.data());

                first =buffer.find_last_of("<")+1;
                last = buffer.find_first_of(">",first);
                coord=buffer.substr(first,(last-first)); // check for exit

                exfirstuser= (buffer.find_first_of("<",1)+1);
                exsecuser = buffer.find_first_of(">",exfirstuser);
                exituser = buffer.substr(exfirstuser,exsecuser-exfirstuser);
                //cout<<exituser<<" "<<exsecuser<<" "<<exfirstuser<<endl;

                if(coord=="exit"||coord =="shutdown")
                {
                    cout<<exituser<<" left the game"<<endl;
                    if (exituser == playero)
                    {
                        totalsend.append("<zeeslag><"+playero+"><"+playert+">"+"<exit>");
                        publisher.send(zmq::buffer(totalsend,totalsend.size()),zmq::send_flags::none);                                          //7+8 send a reply from a composed message
                        //cout<<"send:<zeeslag><"<<playero<<"><"<<playert<<">"<<"<exit>"<<endl;
                        status = false;
                        totalsend.clear();
                        zmq::buffer("\0");

                    }
                    else if(exituser == playert)
                    {
                        totalsend.append("<zeeslag><"+playert+"><"+playero+">"+"<exit>");
                        publisher.send(zmq::buffer(totalsend,totalsend.size()),zmq::send_flags::none);// sending exit to player
                        //cout<<"send:<zeeslag><"<<playert<<"><"<<playero<<">"<<"<exit>"<<endl;
                        status = false;
                        totalsend.clear();
                        zmq::buffer("\0");

                    }
                }
                else
                {
                        cout<<"coordinates"<<coord<<endl;

//                    if(coord.size()!=2)
//                    {
//                        cout<<"coordinates out of range"<<endl;
//                        coordstatus.append("coordinates out of range");
//                        if (exituser == playero)
//                        {
//                            publisher.send(zmq::buffer(coordstatus,coordstatus.size()),zmq::send_flags::none);
//                        }
//                        else if(exituser == playert)
//                        {
//                            publisher.send(zmq::buffer(coordstatus,coordstatus.size()),zmq::send_flags::none);
//                        }
//                        coordstatus.clear();
//                    }
//                    else if(coord.at(0)<97||coord.at(0)>105)
//                    {
//                        cout<<"first coordinate is out of range"<<endl;
//                        coordstatus.append("coordinates out of range");
//                        publisher.send(zmq::buffer(coordstatus,coordstatus.size()),zmq::send_flags::none);
//                        coordstatus.clear();

//                    }
//                    else if(coord.at(1)<'1'||coord.at(1)>'9')
//                    {
//                        cout<<"second coordinate is out of range"<<endl;
//                        coordstatus.append("coordinates out of range");
//                        publisher.send(zmq::buffer(coordstatus,coordstatus.size()),zmq::send_flags::none);
//                        coordstatus.clear();
//                    }
//                    else
//                    {
//                        cout<<"coordinates:"<<coord<<endl;
//                        coordstatus.append("coordinates out of range");
//                        publisher.send(zmq::buffer(coordstatus,coordstatus.size()),zmq::send_flags::none);
//                        coordstatus.clear();
//                    }
                    coord.clear();
                }
                buffer.clear();
                exituser.clear();
                playero.clear();
                playert.clear();

            }
        }
    }
    publisher.close();
    subscriber.close();
    context.shutdown();

}
void adduser()
{
    int first =0;
    int last = 0;

    string rcvbuf;
    zmq::message_t msg;
    //zmq::socket_t publisher(context,ZMQ_PUSH);
    zmq::socket_t subscriber(context,ZMQ_SUB);

   // publisher.connect("tcp://benternet.pxl-ea-ict.be:24041");
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

    subscriber.set(zmq::sockopt::subscribe,"<zeeslag><username>");
    sleep(2);//add a delay so there is enough time subcribing is performed

    cout<<"service started.."<<endl;
    cout<<"subscribing on <zeeslag><username>"<<endl;

    for(;;)
    {
        cout<<"searching for players"<<endl;
        subscriber.recv(msg,zmq::recv_flags::none);                                                                                                            //5+6.second receive + parsing
        rcvbuf=(char *)(msg.data());
        cout<<"subscriber.recv:"<<rcvbuf<<endl;

        first =rcvbuf.find_last_of("<")+1;
        last = rcvbuf.find_first_of(">",first);

        user=rcvbuf.substr(first,(last-first));
        cout<<"player:";
        cout<<user<<endl;
        cout<<endl;
        rcvbuf.clear();
    }
    //publisher.close();
    subscriber.close();
    context.shutdown();

}

void printList()
{
    vector <string> players(0);
    string subzeeslag ="<zeeslag>";
    string subuser = "<username>";
    //string talktouser;
    string opponent;
    string playerone;
    string playertwo;
    string playname;


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

        //---------select player in vector----------------
                if(players.size()>1)
                {
                    opponent= string(players.at(0)); //longest waiting player
            //
                    playerone.append(subzeeslag+"<"+playname+">"+"<your opponent is "+opponent+">");
                    playertwo.append(subzeeslag+"<"+opponent+">"+"<your opponent is "+playname+">");
                    cout<<playname<<" is playing against "<<opponent<<endl;
                    tegenstander = opponent;
                    opponent.clear();
                    playname.clear();
            //Playing players out of choosing list

                    cout <<"\nuser deleted"<<endl;
                    players.erase(players.end());
                    players.erase(players.begin());
                    for(int a =0;a<int(players.size());a++)
                    {
                        cout << int(players.size())-(int(players.size())-a)+1 <<endl;//to check if vector is empty
                    }
                    //zmq::buffer("\0");
                    publisher.send(zmq::buffer(playerone),zmq::send_flags::none);                                                                                       //7+8 send a reply from a composed message
                    cout<<"publisher.send"<<playerone<<endl;
                    publisher.send(zmq::buffer(playertwo),zmq::send_flags::none);
                    cout<<"publisher.send"<<playertwo<<endl;
                    playerone.clear();
                    playertwo.clear();
                    zmq::buffer("\0");

                    status = 1;

                    }
                }
        }
    publisher.close();
    subscriber.close();
    context.shutdown();
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
