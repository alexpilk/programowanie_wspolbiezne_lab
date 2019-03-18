#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char * argv[]) {
    int pid = getpid();
    int steps = atoi(argv[1]);
    printf("Started new process: %d\n", pid);
    for (int step = 1; step < steps + 1; step++) {
        printf(" Child %d step %d\n", pid, step);
        sleep(1);
    }
    exit(0);
}
