#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  //fork
#include <sys/wait.h> //waitpid
#include <errno.h> 


int main() {
    int x = 5;
    pid_t parent = getpid();
    pid_t child = fork();
    if(parent == getpid()) {
        x = 6;
        printf("parent, own pid: %i, parent pid: %i, x: %i\n", getpid(), getppid(), x);
        int status;
        wait(&status);
    } else {
        printf("child, own pid: %i, parent pid: %i, x: %i\n", getpid(), getppid(), x);
    

        sleep(1);
        printf("pid: %i, x: %i\n", getpid(), x);
    }
    return 0;
}
