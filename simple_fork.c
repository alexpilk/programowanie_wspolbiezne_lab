#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    int parent_steps = atoi(argv[1]);
    int child_processes = argc - 2;

    for(int process = 1; process < child_processes + 1; process++) {
        if(fork() == 0) {
            int steps = atoi(argv[process + 1]);
            printf("Started new process: %d\n", process);
            for (int step = 1; step < steps + 1; step++) {
                printf(" Child %d step %d\n", process, step);
                sleep(1);
            }
            exit(process);
        }
    }

    printf("Parent = %d \n", getpid());
    for (int step = 1; step < parent_steps + 1; step++) {
        printf(" Parent step %d\n", step);
        sleep(1);
    }

    for (int process = 1; process < child_processes + 1; process++) {
        int status;
        int pid = wait(&status);
        printf("Proces %d (%d) zakonczony status: %d\n", process, pid, WEXITSTATUS(status));
    }
}
