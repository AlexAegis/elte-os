#include "a.h"

int p[2]; // Pipe descriptor 0 read 1 write
int sighandler_num;
// normal handler
void handler(int signum)
{
    printf(C_RED "%i\t- %i handler called" C_RESET "\n", getpid(), signum);
}

// info loaded handler
static void handler_info(int sig, siginfo_t* si, void* ucontext)
{
    printf(C_RED "%i\t- handler called, got value: %i" C_RESET "\n", getpid(), si->si_value.sival_int);
    sighandler_num = si->si_value.sival_int;
}

int main(int argc, char* argv[])
{

    printf("%i\t- Main start %i\n", getpid(), MEMSIZE);

    signal(SIGUSR1, handler); // User defined signal

    // INFO LOADED SIGNAL
    struct sigaction sigact;
    sigact.sa_sigaction = handler_info; //instead of sa_handler, we use the 3 parameter version
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_SIGINFO; //we need to set the siginfo flag
    sigaction(SIGTERM, &sigact, NULL);

    leader();

    return 0;
}

int write_to_pipe(int num, int* pipe_desc)
{
    printf(C_CYAN "%i\t- feladat fájlból kiolvasva: " C_RESET "\n", getpid());

    write(pipe_desc[1], &num, sizeof(int));
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

int leader()
{

    printf("leader started\n");

    if (pipe(p) == -1)
    {
        perror("Hiba a pipe nyitaskor!\n");
        exit(EXIT_FAILURE);
    }
    pid_a = fork();
    printf("%i\t- " C_RED " FORK CALLED GETPID(): %i, GETPPID(): %i, PID_A: %i PID_B: %i" C_RESET "\n", getpid(), getpid(), getppid(), pid_a, pid_b);
    if (pid_a == -1)
    {
        perror("Fork hiba\n");
        exit(EXIT_FAILURE);
    }
    if (pid_a > 0) // PARENT
    {

        printf(C_CYAN "%i\t- Leader finished" C_RESET "\n", getpid());

        pid_b = fork();
        printf("%i\t- " C_RED " FORK CALLED GETPID(): %i, GETPPID(): %i, PID_A: %i, PID_B: %i " C_RESET "\n", getpid(), getpid(), getppid(), pid_a, pid_b);
        if (pid_b == -1) // ERROR
        {
            perror("Fork hiba\n");
            exit(EXIT_FAILURE);
        }
        if (pid_b > 0) // PARENT
        {
            printf(C_CYAN "%i\t- Game leader innermost." C_RESET "\n");
            wait(NULL);
            wait(NULL);
            printf(C_CYAN "%i\t- Players finished. Shutting down.." C_RESET "\n");
        }
        else
        { // PLAYER 2
            player(2);
            exit(0);
        }
    }
    else // PLAYER 1
    {
        player(1);
        close(p[0]);
        close(p[1]);
        printf(C_YELLOW "%i\t- Player finished" C_RESET "\n", getpid());
        exit(0);
    }
}

int player(int num)
{
    printf("Player %i started \n", num);
    sleep(1);
    exit(0);
}