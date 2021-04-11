#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

void punt(int);
void punt (int value)
{



}

int main( int argc, char * argv[] )
{
        void *context = zmq_ctx_new();
        void *publisher = zmq_socket(context, ZMQ_PUSH);
        void *subscriber = zmq_socket (context, ZMQ_SUB);
        const char answ[] = "startzeeslag";
        char ask[] ="startzeeslag";

        char buf [500];
        zmq_msg_t msg;

        //connect
        int rc = zmq_connect( subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );
        int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");


        for(int i=0; i < 5; i++)
        {
            rc = zmq_msg_init (&msg);
            assert (rc == 0);


            rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, answ, strlen (answ));

            rp = zmq_send(publisher, ask, sizeof(ask), 0);
            printf("ask send\n");
            assert (rp != -1 );//check send
            printf("searching for players");
            int flag =1;
            while(flag == 1)
            {
                rc = zmq_recv (subscriber, buf, sizeof(buf), 0);
                assert(rc != -1);//check status recv

                printf(".");
                sleep(1);

                if(buf > 0)
                {
                printf("\nPlayer found\n");//received messages
                flag = 0;
                memset(&buf,'\0',sizeof(buf));
                }


            }
                printf("\n");







            zmq_msg_close (&msg);
        }

        //free (string);
        zmq_close(subscriber);
        zmq_close(publisher);
        zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
        zmq_ctx_term( context ); //cleaning lady goes to work
        return 0;
}
