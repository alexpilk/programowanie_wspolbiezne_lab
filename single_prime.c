#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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
    // size = ftruncate(fd, sizeof(bufor_t));

    // if (size < 0) {
    // perror("trunc");
    //     exit(-1);
    // }

    bufor_t* wbuf = (bufor_t * ) mmap(0, sizeof(bufor_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (wbuf == NULL) {
        perror("map");
        _exit(-1);
    }
    return wbuf;
}

int is_prime(int number){
    if (number == 1 || number == 0) {
        return 0;
    }

    for(int i = number - 1; i > 1; i--) {
        if(number % i == 0) {
            return 0;
        }
    }
    return 1;
}


int count_primes(int from, int to) {
    int count = 0;
    for (int i = from; i < to; i++) {
        if(is_prime(i)) {
            count++;
        }
    }
    return count;
}


int main(int argc, char * argv[]) {
    printf("Start %s\n", argv[3]);
    bufor_t* buffer = init_buff();
    printf("Buffer ready %s\n", argv[3]);
    int index = atoi(argv[3]);

    buffer->dane[index].from = atoi(argv[1]);
    buffer->dane[index].to = atoi(argv[2]);
    buffer->dane[index].count = count_primes(buffer->dane[index].from, buffer->dane[index].to);

    printf("Writing %d\n", buffer->dane[index].count);

    return 0;
}
