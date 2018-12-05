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

    company();
    /*
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
*/
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

    int _total = -1;
    all_orders = filter(NULL, &_total, -1);
    printf(C_CYAN "%i\t- result count of all: %i" C_RESET "\n", getpid(), _total);
    int _i = 0;
    int current_task = 0;
    int left = _total;

    // simulation of days:
    int current_day = 0;
    while (current_day < _total && current_day < 50) // hard limit in case of something messes up
    {
        printf(C_CYAN "%i\t- " C_RED "DAY %i" C_CYAN " started! Tasks left: %i." C_RESET "\n", getpid(), current_day + 1, left);
        if (left > 0)
        {

            // CREATE A WORKER PROCESS
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
            if (pid > 0) // PARENT - DISPATCH
            {
                printf("%i\t- Dispatch start\n", getpid());
                dispatch_logic(&current_task);
                close(p[0]);
                close(p[1]);
                wait(NULL);
                printf("%i\t- Dispatch finished\n", getpid());
            }
            else // NEW WORKER THREAD
            {
                printf("%i\t- Worker start\n", getpid());
                worker();
                close(p[0]);
                close(p[1]);
                printf("%i\t- Worker finished\n", getpid());
            }
        }
        else
        {

            printf(C_CYAN "%i\t- " C_RED "DAY %i" C_CYAN " No tasks left! Pizza time!" C_RESET "\n", getpid(), current_day + 1);
        }
        current_day++;
    }

    //write_to_pipe(&all_orders[0], p);
    //write_to_pipe(&all_orders[1], p);

    return 0;
}

int dispatch_logic(int* current_task)
{
    printf("%i\t- " C_CYAN " Task id is: %i is for performance: %i" C_RESET "\n", getpid(), all_orders[*current_task].id, all_orders[*current_task].perf);

    struct order _perf_filter;
    _perf_filter.id = -1;
    strcpy(_perf_filter.name, "");
    strcpy(_perf_filter.email, "");
    strcpy(_perf_filter.phone, "");
    _perf_filter.perf = all_orders[*current_task].perf;
    _perf_filter.done = -1;
    printf("%i\t- " C_CYAN " Performance filter created: %i" C_RESET "\n", getpid(), _perf_filter.perf);
    print_order(&_perf_filter);

    int _read_c = -1;
    struct order* _read_result = filter(&_perf_filter, &_read_c, -1);
    int _read_i = 0;

    printf("%i\t- " C_CYAN " Found %i tasks with the same performance" C_RESET "\n", getpid(), _read_c);

    while (_read_i < _read_c && _read_i < WORKER_DAY_MAX && _read_i < 1) // at most two, and at most what we found AND A HARD LIMIT REMOVE IT
    {
        write_to_pipe(&_read_result[_read_i], p);
        _read_i++;
    };

    if (_read_i < 2)
    { // if we only sent 1, pad the 2. out
        printf("NO SECOND ONE \n");
        pause();
    }
    // DISPATCH LOGIC
}

int worker()
{
    sleep(1);

    int target;

    read_from_pipe(&target, p);

    read_from_pipe(&target, p);
}