#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

char FIFO_NAME[] = "fifo";

struct  {
    int from;
    int to;
    int count;
} Report;



int run_single_prime(int from, int to) {
    char from_str[sizeof(int) * 4 + 1];
    char to_str[sizeof(int) * 4 + 1];
    sprintf(from_str, "%d", from);
    sprintf(to_str, "%d", to);
    return execl("./single_prime", "single_prime", from_str, to_str, (char *)0);
}

int main(int argc, char *argv[]){

    int FROM = 0;
    int TO = 5000;
    int range = TO - FROM;
  
    int threads = 4;//atoi(argv[1]);
    int increment = range/threads;

    int total = 0;
    remove(FIFO_NAME);
    if(mkfifo(FIFO_NAME, S_IRUSR | S_IWUSR) < 0) {   
        perror("Pipe");
        return 0; 
    }

    for(int i = 0; i < threads; i++) {

        if(fork() == 0) {  // potomny --------------
            int from = FROM + i * increment;
            int to = from + increment;
            return run_single_prime(from, to);
    	} 
    }
  
    int fifo = open(FIFO_NAME, O_RDWR);
    if (fifo < 0) {
        perror("Couldn't open FIFO");
    }

    for (int thread = 0; thread < threads; thread++) {
        int status;
        wait(&status);
        int result = WEXITSTATUS(status);
        printf("Thread %d returned %d \n", thread, result);
      
        read(fifo, &Report, sizeof(Report));
        printf("Od %d do %d liczb %d\n",Report.from,Report.to,Report.count); 
        total += Report.count;
        printf("Reported %d\n", Report.count);
    }

    printf("Total: %d\n", total);

    close(fifo);
    remove(FIFO_NAME);
    return total;    
}
