#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <mqueue.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char **argv)
{
    int bytes_read;
    mqd_t mq;
    mq = mq_open(QUEUE_NAME, O_RDONLY, 0644);
    CHECK((mqd_t)-1 != mq);

    do {
        ms_type msg;
       
        /* receive the message */
        bytes_read = mq_receive(mq, (char *) &msg, sizeof(msg), NULL);
        if(bytes_read < 0) perror("receive");
        printf("Received: %d %s\n", msg.type, msg.text);
        sleep(1);
    } while (1);

    return 0;
}