#include "a.h"

int p[2]; // Pipe descriptor 0 read 1 write
void handler(int sigNum)
{
    printf("%i\t- Handler called\n", getpid());
}

int main(int argc, char* argv[])
{

    printf("%i\t-Main start %i\n", getpid(), MEMSIZE);

    signal(SIGUSR1, handler);

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
        company();
        close(p[0]);
        close(p[1]);
        wait(NULL);
        printf("%i\t-Parent finished\n", getpid());
    }
    else // CHILD
    {
        printf("%i\t-Child start\n", getpid());
        worker();
        close(p[0]);
        close(p[1]);
        printf("%i\n- Child finished\n", getpid());
    }

    return 0;
}

int company()
{

    int _c = -1;
    int* _count = &_c;
    struct order* _result = filter(NULL, _count, -1);
    printf("%i\t- result count of all: %i\n", getpid(), *_count);
    int _i = 0;

    struct order* _r = &_result[2];
    printf("%i\t- feladat fájlból kiolvasva: \n", getpid());
    print_order(_r);
    write(p[1], &_r->id, sizeof(int));
    pause();
    _r = &_result[1];
    print_order(_r);

    write(p[1], &_r->id, sizeof(int));

    printf("%i\t-Parent waiting for 1 children\n", getpid());

    return 0;
}

int worker()
{

    int buff;

    printf("ge\n");
    read(p[0], &buff, sizeof(int));
    kill(getppid(), SIGUSR1); // unpause parent
    printf("ge\n");

    printf("%i\t-Child got: %i\n", getpid(), buff);

    //close(p[0]);

    read(p[0], &buff, sizeof(int));
    printf("ge\n");

    printf("%i\t-Child got: %i\n", getpid(), buff);
}