#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/mman.h> 
#include <semaphore.h> 

char FIFO_NAME[] = "fifo";

typedef struct {
    int from;
    int to;
    int count;
} Report;


typedef struct { 
    int wymiar; 
    Report dane[4]; 
} bufor_t;


bufor_t* init_buff(){
    int fd, size;
    fd = shm_open("bufor", O_RDWR | O_CREAT, 0774);
    if (fd == -1) {
    perror("open");
        exit(-1);
    }
    printf("fd: %d\n", fd);
    size = ftruncate(fd, sizeof(bufor_t));

    if (size < 0) {
    perror("trunc");
        exit(-1);
    }

    bufor_t* wbuf = (bufor_t * ) mmap(0, sizeof(bufor_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (wbuf == NULL) {
        perror("map");
        _exit(-1);
    }
    return wbuf;
}


int run_single_prime(int from, int to, int i) {
    char from_str[sizeof(int) * 4 + 1];
    char to_str[sizeof(int) * 4 + 1];
    char index[sizeof(int) * 4 + 1];
    sprintf(from_str, "%d", from);
    sprintf(to_str, "%d", to);
    sprintf(index, "%d", i);
    return execl("./single_prime", "single_prime", from_str, to_str, index, (char *)0);
}

int main(int argc, char *argv[]){

    int FROM = 0;
    int TO = 5000;
    int range = TO - FROM;
  
    int threads = 4;//atoi(argv[1]);
    int increment = range/threads;

    int total = 0;


    shm_unlink("bufor");
    bufor_t* buffer = init_buff();

    for(int i = 0; i < threads; i++) {

        if(fork() == 0) {  // potomny --------------
            int from = FROM + i * increment;
            int to = from + increment;
            return run_single_prime(from, to, i);
    	} 
    }

    for (int thread = 0; thread < threads; thread++) {
        int status;
        wait(&status);
        int result = WEXITSTATUS(status);
        printf("Thread %d returned %d \n", thread, result);
      
        // read(fifo, &Report, sizeof(Report));
        Report* report = &buffer->dane[thread];
        printf("Od %d do %d liczb %d\n",report->from,report->to,report->count); 
        total += report->count;
        printf("Reported %d\n", report->count);
    }

    printf("Total: %d\n", total);

    return total;    
}
