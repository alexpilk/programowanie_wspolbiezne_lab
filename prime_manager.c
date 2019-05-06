#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

char OUTPUT_FILE_NAME[] = "wynik.bin";

struct  {
    int from;
    int to;
    int count;
} Report;

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

int main(int argc, char *argv[]){

    int FROM = 0;
    int TO = 100;
    int range = TO - FROM;
  
    int threads = 2;//atoi(argv[1]);
    int increment = range/threads;
    int pipe_files[2];      
    pipe(pipe_files); 

                int total = 0;
    
    for(int i = 0; i < threads; i++) {
        int from = FROM + i * increment;
        int to = from + increment;

        if(fork() == 0) {  // potomny --------------
            printf("Counting primes from %d to %d\n", from, to);
            close(pipe_files[0]);
            // ile liczb jest od FROM do TO
            Report.from = from;
            Report.to = to;
            Report.count = count_primes(from, to);
            printf("Counted %d primes from %d to %d\n", Report.count, from, to);

            write(pipe_files[1], &Report, sizeof(Report));
            close(pipe_files[1]);
            return 0;
            // int from = FROM + i * increment;
            // int to = from + increment;
            // char from_str[sizeof(int) * 4 + 1];
            // char to_str[sizeof(int) * 4 + 1];
            // sprintf(from_str, "%d", from);
            // sprintf(to_str, "%d", to);
            // return execl("./single_prime", "single_prime", from_str, to_str, (char *)0);
    	} 
    }

    close(pipe_files[1]);
    for (int thread = 0; thread < threads; thread++) {
        int status;
        wait(&status);
        int result = WEXITSTATUS(status);
        printf("Thread %d returned %d \n", thread, result);
        read(pipe_files[0], &Report, sizeof(Report));
        printf("Od %d do %d liczb %d\n",Report.from,Report.to,Report.count); 
        total += Report.count;
        printf("Reported %d\n", Report.count);
    }
    printf("Total: %d\n", total);
    return total;    
}
    

    // for (int thread = 0; thread < threads; thread++) {
    //     int status;
    //     wait(&status);
    //     int result = WEXITSTATUS(status);
    //     printf("Thread %d returned %d \n", thread, result);
    // }

    // struct Report report;
    // int total = 0;
    // int output_file = open(OUTPUT_FILE_NAME, O_RDONLY);
    // if(output_file < 0) {
    //     perror("Could not open output file");
    // }

    // for (int i = 0; i < threads; i++) {
    //     int r = read(output_file, &report, sizeof(report));
    //     printf("%d\n", report.count);
    //     total += report.count;
    // }

    // close(output_file);

    // printf("Total: %d\n", total);

