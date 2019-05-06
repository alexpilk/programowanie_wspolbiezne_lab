#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

char OUTPUT_FILE_NAME[] = "wynik.bin";

struct Report {
    int from;
    int to;
    int count;
};

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

    struct Report report;
    int total = 0;
    int output_file = open(OUTPUT_FILE_NAME, O_RDONLY);
    if(output_file < 0) {
        perror("Could not open output file");
    }

    for (int i = 0; i < threads; i++) {
        int r = read(output_file, &report, sizeof(report));
        printf("%d\n", report.count);
        total += report.count;
    }

    close(output_file);

    printf("Total: %d\n", total);
}
