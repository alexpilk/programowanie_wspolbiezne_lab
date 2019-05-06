#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char FIFO_NAME[] = "fifo";


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


struct Report {
    int from;
    int to;
    int count;
};


int main(int argc, char * argv[]) {
    struct Report report;

    report.from = atoi(argv[1]);
    report.to = atoi(argv[2]);
    report.count = count_primes(report.from, report.to);

    int handle = open(FIFO_NAME, O_WRONLY | O_APPEND | O_CREAT, 0777);
    if(handle < 0) {
        perror("open");
        return 0;
    }

    int lock;
    lock = lockf(handle, F_LOCK, 0);
    if(lock < 0) {
        perror("lockf");
        return 0;
    }

    printf("Writing %d\n", report.count);
    int wr = write(handle, &report, sizeof(report));
    if(wr < 0) {
        perror("write");
    }
    printf("od %d do %d liczb %d\n", report.from, report.to, report.count);
    int release = lockf(handle, F_ULOCK, 0);
    close(handle);
}
