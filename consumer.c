#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "common.h"

int main(int argc, char ** argv) {
  bufor_t* wbuf = init_buff();

  for (int i = 0; i < 10; i++) {
    sem_wait( & (wbuf -> full));
    sem_wait( & (wbuf -> mutex));
    printf("Konsument - cnt: %d odebrano %s\n", wbuf -> cnt, wbuf -> buf[wbuf -> tail]);
    wbuf -> cnt--;
    wbuf -> tail = (wbuf -> tail + 1) % BSIZE;
    sem_post( & (wbuf -> mutex));
    sem_post( & (wbuf -> empty));
    sleep(1);
  }

  sem_close( & (wbuf -> mutex));
  sem_close( & (wbuf -> empty));
  sem_close( & (wbuf -> full));
}