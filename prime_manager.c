#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>



int main(int argc, char *argv[]){

    int threads = atoi(argv[1]);
    int increment = 500;

    for(int i = 0; i < threads; i++) {
        if(fork() == 0) {
            int from = i * increment;
            int to = from + increment;
            char from_str[sizeof(int) * 4 + 1];
            char to_str[sizeof(int) * 4 + 1];
            sprintf(from_str, "%d", from);
            sprintf(to_str, "%d", to);
            execl("./d", "d", from_str, to_str, (char *)0);
    	}
    }

    int total = 0;
    for (int thread = 0; thread < threads; thread++) {
        int status;
        wait(&status);
        int result = WEXITSTATUS(status);
        total += result;
        printf("Thread %d returned %d, total = %d\n", thread, result, total);
    }
}
