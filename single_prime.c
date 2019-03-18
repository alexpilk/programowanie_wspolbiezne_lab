#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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


int main(int argc, char * argv[]) {
    int from = atoi(argv[1]);
    int to = atoi(argv[2]);
    int count = 0;
    
    for (int i = from; i < to; i++) {
        if(is_prime(i)) {
            count++;
        }
    }
    
    exit(count);
}
