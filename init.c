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
    mqd_t mq;
    ms_type msg;

    struct mq_attr attr;
    char buffer[MAX_SIZE + 1];
    int must_stop = 0;
    ssize_t bytes_read;

    /* initialize the queue attributes */
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(msg);
    attr.mq_curmsgs = 0;
  
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    CHECK((mqd_t)-1 != mq);

    getchar();

    /* cleanup */
    CHECK((mqd_t)-1 != mq_close(mq));
    CHECK((mqd_t)-1 != mq_unlink(QUEUE_NAME));

    return 0;
}