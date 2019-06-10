#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

///////////
#define SIZE 4 // Liczba pozycji (napisow) w buforze
#define LSIZE 80 // Dlugosc napisu
char buf[SIZE][LSIZE]; // Bufor na napisy
int head = 0; // Tutaj wpisujemy do bufora nowy element
int tail = 0; // Stad pobieramy element z bufora
int cnt = 0; // Liczba elementow w buforze
pthread_cond_t empty = PTHREAD_COND_INITIALIZER; // Tu czekamy gdy brak miejsca w buforze
pthread_cond_t full = PTHREAD_COND_INITIALIZER; // Tu czekamy gdy brak rekordow w buforze
pthread_mutex_t mutex; // Wzajemne wykluczanie
////////////

int wynik;

void* consume(void* arg)
{
    while (1) {
        pthread_mutex_lock(&mutex);

        while (cnt == 0) {
            printf("Buffer empty\n");
            pthread_cond_wait(&empty, &mutex);
        }
        printf("Consumed \n");
        printf(buf[tail]);
        if (tail == SIZE - 1) {
            tail = 0;
        }
        else {
            tail++;
        }
        cnt--;
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        sleep(rand() % 2);
    }
    pthread_exit((void*)&wynik);
}

void* produce(void* arg)
{
    int i = 0;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (cnt == SIZE) {
            printf("Buffer full\n");
            pthread_cond_wait(&full, &mutex);
        }
        printf("Produced\n");
        sprintf(buf[head], "Producer %d, step %d\n", (int) arg, i);
        if (head == SIZE - 1) {
            head = 0;
        }
        else {
            head++;
        }
        cnt++;
        pthread_cond_signal(&empty);

        pthread_mutex_unlock(&mutex);
        i++;
        sleep(rand() % 2);
    }
    pthread_exit((void*)&wynik);
}

int main(int argc, char* argv[])
{
    pthread_t producer_1_id = 1;
    pthread_t producer_2_id = 2;
    pthread_t consumer_id = 3;
    int i, status;
    void** statp;
    printf("HI");

    pthread_create(&producer_1_id, NULL, produce, (void*)(producer_1_id));
    pthread_create(&producer_2_id, NULL, produce, (void*)(producer_2_id));
    pthread_create(&consumer_id, NULL, consume, (void*)(consumer_id));

    pthread_join(producer_1_id, (void*)&statp);
    pthread_join(producer_2_id, (void*)&statp);
    pthread_join(consumer_id, (void*)&statp);

    return 0;
}