#include "a.h"

int p[2]; // Pipe descriptor 0 read 1 write

int main(int argc, char* argv[])
{

    printf("%i\t-Main start %i\n", getpid(), MEMSIZE);

    if (pipe(p) == -1)
    {
        perror("Hiba a pipe nyitaskor!\n");
        exit(EXIT_FAILURE);
    }
    pid = fork();

    if (pid == -1)
    {
        perror("Fork hiba\n");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) // PARENT
    {
        printf("%i\t-Parent start\n", getpid());
        close(p[0]);
        write(p[1], "Hello", MEMSIZE);
        close(p[1]);
        // fflush(NULL);
        printf("%i\t-Parent waiting for 1 children\n", getpid());
        wait(NULL);
        printf("%i\t-Parent finished\n", getpid());
    }
    else // CHILD
    {
        sleep(1);
        printf("%i\t-Child start\n", getpid());
        close(p[1]);
        char buff[MEMSIZE];
        printf("ge\n");
        read(p[0], buff, MEMSIZE);
        printf("ge\n");

        printf("%i\t-Child got: %s\n", getpid(), buff);
        close(p[0]);
        printf("ge\n");
    }

    return 0;
}

int company()
{
    // close(p[0]);
    // write(p[1], "Hellohahahaha", MEMSIZE);
    // close(p[1]);
    return 0;
}