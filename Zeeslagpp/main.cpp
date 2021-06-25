#include <iostream>
#include <thread>
#include <list>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.hpp>

using namespace std;
zmq::context_t context(1);
string user;
string finalscore;
string tegenstander;
string men;
string menscore;
bool status = 0;
//
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
    int firstscore;
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
                        cout<<"test eval"<<endl;
                        totalsend.append("<zeeslag><"+playero+"><"+playert+">"+"<exit>");
                        publisher.send(zmq::buffer(totalsend,totalsend.size()),zmq::send_flags::none);                                          //7+8 send a reply from a composed message
                        //cout<<"send:<zeeslag><"<<playero<<"><"<<playert<<">"<<"<exit>"<<endl;
                        status = false;
                        totalsend.clear();
                        zmq::buffer("\0");

                    }
                    else if(exituser == playert)
                    {
                        cout<<"test eval"<<endl;
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
        //recv "<zeeslag><username><+"username"+>

        //first =rcvbuf.find_last_of("<")+1;
        first = rcvbuf.find_first_of("<",19)+1;
        last = rcvbuf.find_first_of(">",first);

        int firstscore = rcvbuf.find_last_of("<")+1;
        int lastscore = rcvbuf.find_first_of(">",firstscore);

        //score
        finalscore= rcvbuf.substr(firstscore,(lastscore-firstscore));
        user=rcvbuf.substr(first,(last-first));
        cout<<"player:";
        cout<<user<<" score:"<<finalscore<<endl;
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
    vector <string> score(0);
    string subzeeslag ="<zeeslag>";
    string subuser = "<username>";
    //string talktouser;
    string opponent;
    string opponentscore;
    string playerone;
    string playertwo;
    string playname;
    string playscore;


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
            score.push_back(finalscore);
            playname = user;
            playscore = finalscore;
            men = user;
            menscore=finalscore;
            cout <<"user added"<<endl;
            for(int a =0;a<int(players.size());a++)
            {
                cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"score:"<<string(score.at(a))<<endl;
            }
            user.clear();
            finalscore.clear();

        //---------select player in vector----------------
                if(players.size()>1)
                {
                    opponent= string(players.at(0)); //longest waiting player
                    opponentscore=string(score.at(0));

                    //playname = string(players.at(1));//first waiting player.

                    playerone.append(subzeeslag+"<"+playname+">"+"<your opponent is "+opponent+"><"+opponentscore+">");
                    playertwo.append(subzeeslag+"<"+opponent+">"+"<your opponent is "+playname+"><"+playscore+">");
                    cout<<playname<<" is playing against "<<opponent<<endl;
                    tegenstander = opponent;

            //Playing players out of choosing list


                    //zmq::buffer("\0");
                    if(score.at(0)>=score.at(1))
                    {
                        cout<<string(players.at(1))<<" has lowest score he begins";
                        publisher.send(zmq::buffer(playerone),zmq::send_flags::none);                                                                                       //7+8 send a reply from a composed message
                        cout<<"publisher.send"<<playerone<<endl;
                        publisher.send(zmq::buffer(playertwo),zmq::send_flags::none);
                        cout<<"publisher.send"<<playertwo<<endl;
                    }
                    else
                    {
                        cout<<string(players.at(0))<<" has lowest score";
                        publisher.send(zmq::buffer(playertwo),zmq::send_flags::none);
                        cout<<"publisher.send"<<playertwo<<endl;
                        publisher.send(zmq::buffer(playerone),zmq::send_flags::none);                                                                                       //7+8 send a reply from a composed message
                        cout<<"publisher.send"<<playerone<<endl;

                    }

                    cout <<"\nuser deleted"<<endl;
                    players.erase(players.end());
                    players.erase(players.begin());
                    score.erase(score.end());
                    score.erase(score.begin());
                    for(int a =0;a<int(players.size());a++)
                    {
                        cout << int(players.size())-(int(players.size())-a)+1 <<endl;//to check if vector is empty
                    }
                    opponent.clear();
                    playname.clear();
                    opponentscore.clear();
                    playscore.clear();
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
