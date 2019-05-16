#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

#include "common.h"


int main(int argc, char **argv)
{
    mqd_t mq;

    mq = mq_open(QUEUE_NAME, O_WRONLY);
    CHECK((mqd_t)-1 != mq);

    for(int i=0; i<10; i++) {
        ms_type msg;
        msg.pnr = 1;
        msg.type = PROD;
        sprintf(msg.text,"Producent %d krok %d", msg.pnr, i);
        // Przeslanie komunikatu do kolejki
        CHECK(0 <= mq_send(mq, (const char*) &msg, sizeof(msg), 0));
        printf("Sent %d\n", i);
        sleep(1); 
    }

    CHECK((mqd_t)-1 != mq_close(mq));

    return 0;
}