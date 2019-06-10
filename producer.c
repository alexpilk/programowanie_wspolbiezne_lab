#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define threads 4

typedef struct {
    int from;
    int to;
    int count;
} Report;
Report reports[threads];
pthread_t id[4]; 

int wynik;


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

void* produce(void* arg)
{
    Report report = reports[(int) arg];

  printf("prining %d to %d\n", report.from, report.to);
    for (int i = report.from; i < report.to; i++) {
        if(is_prime(i)) {
            report.count++;
        }
    }
    reports[(int) arg].count = report.count;
  printf("got %d\n", report.count);
    pthread_exit((void*)&report.count);
}

int main(int argc, char* argv[])
{
    int FROM = 0;
    int TO = 5000;
    int range = TO - FROM;

    int increment = range/threads;
    printf("gut");


    for(int i = 0; i < threads; i++) {
      // pthread_t id = i;
        reports[i].from = FROM + i * increment;
        reports[i].to = reports[i].from + increment;
        pthread_create(&id[i], NULL, produce, (void*)(i));
    }

    int total = 0;

    for(int i = 0; i < threads; i++) {
      // pthread_t id = i;
        void** statp;
        pthread_join(id[i], (void*)&statp);
        // total += (int)*statp;
    }

    for(int i = 0; i < threads; i++) {
        Report report = reports[i];     
        total += report.count;

    }
    printf("%d\n", total);


    // pthread_t producer_1_id = 1;
    // pthread_t producer_2_id = 2;
    // pthread_t consumer_id = 3;
    // int i, status;
    // void** statp;
    // printf("HI");

    // pthread_create(&producer_1_id, NULL, produce, (void*)(producer_1_id));
    // pthread_create(&producer_2_id, NULL, produce, (void*)(producer_2_id));
    // pthread_create(&consumer_id, NULL, consume, (void*)(consumer_id));

    // pthread_join(producer_1_id, (void*)&statp);
    // pthread_join(producer_2_id, (void*)&statp);
    // pthread_join(consumer_id, (void*)&statp);

    return 0;
}