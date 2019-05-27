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
    bufor_t* wbuf = init_buff();

    for(int i=0; i<10; i++) {
       sem_wait(&(wbuf->empty)); 
       sem_wait(&(wbuf->mutex)); 
       sprintf(wbuf->buf[wbuf->head],"Komunikat %d",i);  
       printf("Producent - cnt:%d head: %d tail: %d\n",  
               wbuf-> cnt,wbuf->head,wbuf->tail); 
       wbuf-> cnt ++; 
       wbuf->head = (wbuf->head +1) % BSIZE; 
       sem_post(&(wbuf->mutex)); 
       sem_post(&(wbuf->full)); 
       sleep(1);
    }

    return 0;
}