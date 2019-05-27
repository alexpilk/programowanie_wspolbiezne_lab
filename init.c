#include <sys/mman.h> 
#include <fcntl.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

#include "common.h"

int main(int argc, char **argv)
{
    // mqd_t mq;
    // ms_type msg;

    // struct mq_attr attr;
    // char buffer[MAX_SIZE + 1];
    // int must_stop = 0;
    // ssize_t bytes_read;

    // /* initialize the queue attributes */
    // attr.mq_flags = 0;
    // attr.mq_maxmsg = 10;
    // attr.mq_msgsize = sizeof(msg);
    // attr.mq_curmsgs = 0;
  
    // mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, &attr);
    // CHECK((mqd_t)-1 != mq);

    // getchar();

    // /* cleanup */
    // CHECK((mqd_t)-1 != mq_close(mq));
    // CHECK((mqd_t)-1 != mq_unlink(QUEUE_NAME));
     int i,stat,k, pid, size, fd,res; 
     char c; 
     // Utworzenie segmentu --------------------------- 
     shm_unlink("bufor"); 
     bufor_t* wbuf = init_buff(); 
     // Inicjacja obszaru -------------------------------- 
     wbuf-> cnt = 0; 
     wbuf->head = 0; 
     wbuf->tail = 0; 
     if(sem_init(&(wbuf->mutex),1,1)){ 
       perror("mutex"); _exit(0);  
     } 
     if(sem_init(&(wbuf->empty),1,BSIZE)) {  
       perror("empty"); _exit(0);  
     } 
     if(sem_init(&(wbuf->full),1,0)) {  
       perror("full"); _exit(0);  
     } 

    return 0;
}