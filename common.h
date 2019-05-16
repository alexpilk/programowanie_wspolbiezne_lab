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

int PROD = 1;
int CONS = 2;

typedef struct  { 
    int  type;        /* typ procesu:  1 PROD, 2 KONS  */ 
    int  pnr ;        /* numer procesu     */   
    char text[100];  /* tekst  komunikatu */ 
} ms_type; 
