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
        char ask[] ="<zeeslag>";
        int size = 0;
        char *total;
        int delaytime =10; //delay in sec
        char buf [500];
        zmq_msg_t msg;
        char username[100];

        //connect
        int rc = zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );
        int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");

        //-----clear all buffers
        memset(&username,'\0',sizeof(username));
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



            rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, answ, strlen (answ));
            rp = zmq_send(publisher, total, strlen(total), 0);

            //printf("ask send\n");
            assert (rp != -1 );//check send
            printf("searching for players");
            int counter =0;

            while(flag == 1)
            {
                rc = zmq_recv (subscriber, buf, sizeof(buf), 0);
                assert(rc != -1);//check status recv

                printf(".");
                delay(1);
                //printf("%d",flag);
                counter = counter +1;

                if (counter>delaytime)
                {
                    flag = 0;
                    printf("\nno players found");
                }

                if(buf > 0)
                {
                      printf("%s",buf);
                    flag = 0;
                    memset(&buf,'\0',sizeof(buf));
                }


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
