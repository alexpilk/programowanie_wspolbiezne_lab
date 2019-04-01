#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

char output_file[] = "wynik.bin";

int main(int argc, char *argv[]){

    int FROM = 0;
    int TO = 100;
    int range = TO - FROM;

    int threads = 2;//atoi(argv[1]);
    int increment = range/threads;
    
    for(int i = 0; i < threads; i++) {
        if(fork() == 0) {
            int from = FROM + i * increment;
            int to = from + increment;
            char from_str[sizeof(int) * 4 + 1];
            char to_str[sizeof(int) * 4 + 1];
            sprintf(from_str, "%d", from);
            sprintf(to_str, "%d", to);
            return execl("./single_prime", "single_prime", from_str, to_str, (char *)0);
    	}
    }

    for (int thread = 0; thread < threads; thread++) {
        int status;
        wait(&status);
        int result = WEXITSTATUS(status);
        printf("Thread %d returned %d \n", thread, result);
    }

    int total = 0;
    int buffer[threads];
    int ptr = open(output_file, O_RDONLY);  // r for read, b for binary

    read(ptr, buffer, sizeof(buffer)); // read 10 bytes to our buffer
    close(ptr);
    for (int i = 0; i < threads; i++) {
        printf("%d\n", buffer[i]);
        total += buffer[i];
    }
    printf("Total: %d\n", total);
    // int total = 0;
    // for (int thread = 0; thread < threads; thread++) {
    //     int status;
    //     wait(&status);
    //     int result = WEXITSTATUS(status);
    //     total += result;
    //     printf("Thread %d returned %d, total = %d\n", thread, result, total);
}
