#include "a.h"

int p[2]; // Pipe descriptor 0 read 1 write

void handler(int signum)
{
    printf("%i\t- %i handler called\n", getpid(), signum);
}

int main(int argc, char* argv[])
{

    printf("%i\t- Main start %i\n", getpid(), MEMSIZE);

    signal(SIGUSR1, handler); // User defined signal

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
        printf("%i\t- Parent start\n", getpid());
        company();
        close(p[0]);
        close(p[1]);
        wait(NULL);
        printf("%i\t- Parent finished\n", getpid());
    }
    else // CHILD
    {
        printf("%i\t- Child start\n", getpid());
        worker();
        close(p[0]);
        close(p[1]);
        printf("%i\t- Child finished\n", getpid());
    }

    return 0;
}

int write_to_pipe(struct order* o, int* pipe_desc)
{
    printf(C_CYAN "%i\t- feladat fájlból kiolvasva: " C_RESET "\n", getpid());
    print_order(o);
    write(pipe_desc[1], &o->id, sizeof(int));
    printf(C_CYAN "%i\t- task written into the pipe. Pausing..." C_RESET "\n", getpid());
    pause();
    printf(C_CYAN "%i\t- Unpaused" C_RESET "\n", getpid());
    return 0;
}

int read_from_pipe(int* target, int* pipe_desc)
{
    printf(C_YELLOW "%i\t- Reading from pipe then unpausing parent." C_RESET "\n", getpid());
    read(pipe_desc[0], target, sizeof(int));
    kill(getppid(), SIGUSR1); // unpause parent
    printf(C_YELLOW "%i\t- Read from pipe, got: %i" C_RESET "\n", getpid(), *target);
}

int company()
{

    int _c = -1;
    int* _count = &_c;
    struct order* _result = filter(NULL, _count, -1);
    printf("%i\t- result count of all: %i\n", getpid(), *_count);
    int _i = 0;
    /*
    // simulation of days:
    int current = 0;
    while (current < _count || current < 100){ // hard limit in case of something messes up

    }*/

    struct order* _r = &_result[2];

    write_to_pipe(&_result[0], p);
    write_to_pipe(&_result[1], p);

    return 0;
}

int worker()
{

    int target;

    read_from_pipe(&target, p);
    read_from_pipe(&target, p);
}