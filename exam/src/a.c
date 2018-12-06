#include "a.h"

int p_a[2]; // Pipe descriptor 0 read 1 write
int p_b[2]; // Pipe descriptor 0 read 1 write
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

int leader()
{

    printf("leader started\n");

    if (pipe(p_a) == -1)
    {
        perror("Hiba a pipe nyitaskor!\n");
        exit(EXIT_FAILURE);
    }

    if (pipe(p_b) == -1)
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

            game_logic();
            wait(NULL);
            wait(NULL);
            printf(C_CYAN "%i\t- Players finished. Shutting down.." C_RESET "\n");
        }
        else
        { // PLAYER 2
            player(2);
            close(p_b[0]);
            close(p_b[1]);
            printf(C_YELLOW "%i\t- Player 2 finished" C_RESET "\n", getpid());
            exit(0);
        }
    }
    else // PLAYER 1
    {
        player(1);
        close(p_a[0]);
        close(p_a[1]);
        printf(C_YELLOW "%i\t- Player 1 finished" C_RESET "\n", getpid());
        exit(0);
    }
    return 0;
}

int game_logic()
{
    printf("gamelogic start\n");
    pause();
    printf("gamelogic one player in\n");
    pause();
    printf("gamelogic two player in\n");
    char name_buffer[5];
    printf(C_YELLOW "%i\t- Reading from pipe then unpausing parent." C_RESET "\n", getpid());
    read(p_a[0], &name_buffer, sizeof(char) * 5);
    kill(pid_a, SIGUSR1); // unpause parent
    printf(C_YELLOW "%i\t- Read from pipe, got: %s" C_RESET "\n", getpid(), name_buffer);

    printf(C_YELLOW "%i\t- Reading from pipe then unpausing parent." C_RESET "\n", getpid());
    read(p_b[0], &name_buffer, sizeof(char) * 5);
    kill(pid_b, SIGUSR1); // unpause parent
    printf(C_YELLOW "%i\t- Read from pipe, got: %s" C_RESET "\n", getpid(), name_buffer);
}

int player(int num)
{
    printf("Player %i started \n", num);
    sleep(1);
    kill(getppid(), SIGUSR1);
    char* name = "feri";

    printf(C_YELLOW "%i\t- pipe write: " C_RESET "\n", getpid());
    if (num == 1)
    {
        write(p_a[1], name, sizeof(char) * 5);
    }
    else
    {
        write(p_b[1], name, sizeof(char) * 5);
    }

    printf(C_YELLOW "%i\t- name %s written into the pipe. Pausing..." C_RESET "\n", getpid(), name);
    pause();
    printf(C_YELLOW "%i\t- Unpaused" C_RESET "\n", getpid());

    return 0;
}