#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char output_file[] = "wynik.bin";


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


struct Report {
    int from;
    int to;
    int count;
};


int main(int argc, char * argv[]) {
    // int from = atoi(argv[1]);
    // int to = atoi(argv[2]);
    struct Report report;

    report.from = atoi(argv[1]);
    report.to = atoi(argv[2]);
    report.count = 0;
    
    for (int i = report.from; i < report.to; i++) {
        if(is_prime(i)) {
            report.count++;
        }
    }

    int handle = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0777);
    if(handle < 0) {
        perror("open");
        return 0;
    }
    int lock;
    lock = lockf(handle, F_LOCK, 0);
    // printf("Lock: %d", lock);
    if(lock < 0) {
        perror("lockf");
        return 0;
    }
    // sleep(2);
    // printf("lock: %d\n", lock);
    printf("Writing %d\n", report.count);
    int wr = write(handle, &report, sizeof(report));
    if(wr < 0) {
        perror("write");
    }
    int release = lockf(handle, F_ULOCK, 0);
    // printf("lock released: %d\n", release);
    close(handle);
    printf("od %d do %d liczb %d\n", report.from, report.to, report.count);
    
    struct Report report2;
    int reopen = open(output_file, O_RDONLY);
    read(reopen, &report2, sizeof(report2));
    printf("od %d do %d liczb %d\n", report2.from, report2.to, report2.count);
    close(reopen);
}
