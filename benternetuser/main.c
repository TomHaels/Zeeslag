#include <stdio.h>
#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

int main( int argc, char * argv[] )
{
        void *context = zmq_ctx_new();
        void *publisher = zmq_socket(context, ZMQ_PUSH);
        void *subscriber = zmq_socket (context, ZMQ_SUB);
        char st[20];
        char *tekst;
        int size= 0;
        zmq_msg_t msg;


        //connect

        int rp = zmq_connect(publisher, "tcp://benternet.pxl-ea-ict.be:24041");
        int rc = zmq_connect(subscriber, "tcp://benternet.pxl-ea-ict.be:24042" );
        rc = zmq_msg_init (&msg);
        assert (rc == 0);

        while(1)
        {
            printf("string to benternet:");
            gets(st);
            printf("\n");
            size = strlen(st);
            tekst = malloc(size);
            rp = zmq_send(publisher, tekst, strlen(tekst), 0);// send <zeeslag>username
            assert (rp != -1 );//check send
            memset(&tekst,'\0',strlen(tekst));
        }
        zmq_msg_close (&msg);


        //free (&tekst);
        zmq_close(publisher);

        zmq_ctx_shutdown( context ); //optional for cleaning lady order (get ready you l*zy f*ck)
        zmq_ctx_term( context ); //cleaning lady goes to work
        return 0;
}
