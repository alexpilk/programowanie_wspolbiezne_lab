#include <semaphore.h>
#include <stdlib.h>
#ifndef COMMON_H_
#define COMMON_H_

#define QUEUE_NAME  "/test_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
            perror(#x); \
            exit(-1); \
        } \
    } while (0) \


#endif /* #ifndef COMMON_H_ */

#define BSIZE     4   // Rozmiar bufora 
#define LSIZE    80   //  Dlugosc linii 

int PROD = 1;
int CONS = 2;

typedef struct  { 
    int  type;        /* typ procesu:  1 PROD, 2 KONS  */ 
    int  pnr ;        /* numer procesu     */   
    char text[100];  /* tekst  komunikatu */ 
} ms_type; 

typedef struct { 
    int head; // Tutaj producent wstawia nowy element 
    int tail; // Stąd pobiera element konsument 
    int cnt;  // Liczba elementów w buforze  
    sem_t mutex; // Semafor chroniący sekcję krytyczną
    sem_t empty; // Semafor wstrzymujący producenta 
    sem_t full;  // Semafor wstrzymujący konsumenta 
    char buf[BSIZE][LSIZE]; 
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