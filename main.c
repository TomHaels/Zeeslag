#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

int main( int argc, char * argv[] )
{
        void *context = zmq_ctx_new();
        void *publisher = zmq_socket(context, ZMQ_PUSH);
        void *subscriber = zmq_socket (context, ZMQ_SUB);
        const char answ[] = "<zeeslag>";
        char usern[]="<zeeslag><username>";
        char ask[] ="<zeeslag>";

        int size = 0;
        char *total;
        int delaytime =10; //delay in sec
        char buf[30];
        zmq_msg_t msg;
        char username[100];

        //connect
        int rc = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );
        int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
        //int rc = zmq_connect(subscriber, "tcp://benternet.backup.pxl-ea-ict.be:24042" );
        //int rp = zmq_connect(publisher, "tcp://benternet.backup.pxl-ea-ict.be:24041");
        //-----clear all buffers

        memset(&username,'\0',sizeof(username));
        memset(&buf,'\0',sizeof(buf));
        //memset(&total,'\0',sizeof(total));

        //----player username
        printf("Username:");
        gets(username);
        //printf("%s\n",username);

        //------memory optimization total buffer
        size= strlen(username)+sizeof(ask);
        total = realloc(&total,size);
        total = malloc(size);

        memset(&total,'\0',strlen(total));
        strcat(total,ask);
        //printf("%s",total);
        strcat(total,username);
        printf("%s\n",total);
        printf("sizeof total %llu strln totel %llu\n",sizeof(total),strlen(total));

        int flag =1;
            rc = zmq_msg_init (&msg);
            assert (rc == 0);

            rp = zmq_send(publisher, total, strlen(total), 0);// send <zeeslag>username
            rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, answ, strlen (answ)); //subsricbe on <zeeslag>

            assert (rp != -1 );//check send

            printf("searching for players");
            int counter =0;

            while(flag == 1)
            {
                int timeout = 5000; // time for recv
                zmq_setsockopt (subscriber, ZMQ_RCVTIMEO, &timeout, sizeof (int)); // sockopt for timeout recv

                rc = zmq_recv (subscriber, buf, sizeof(buf), 0);
                assert(rc != -1);//check status recv

                //rp = zmq_send(publisher, buf, strlen(buf),0);

                printf(".");
                delay(1);
                //printf("%d",rc);
                counter = counter +1;

                if (counter>delaytime)
                {
                    flag = 0;
                    printf("\nno players found");
                }

                if(buf > 0)
                {
                    printf("\n player found %s strlen %llu sizeof %llu",buf,strlen(buf),sizeof(buf));
                    flag = 0;
                    memset(&buf,'\0',sizeof(buf));
                    rc = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, buf, strlen(buf));
                }

//                string name;
//                vector <string> players(0);
//                while(1)
//                {
//                    std:: cin >> name;

//                    players.push_back(name);
//                    //print players list + location.
//                    for(int a =0;a<int(players.size());a++)
//                    {
//                        cout << int(players.size())-(int(players.size())-a)+1 <<"\t"<< string(players.at(a))<<"\t"<<"sizeof:"<< string(players.at(a)).size()<<"\t"<<"length:"<< string(players.at(a)).length()<<"\n";
//                    }

//                }

            }
                printf("\n");
        zmq_msg_close (&msg);


        //free (string);
        zmq_close(subscriber);
        zmq_close(publisher);
        free(total);

        zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
        zmq_ctx_term( context ); //cleaning lady goes to work
        return 0;
}
