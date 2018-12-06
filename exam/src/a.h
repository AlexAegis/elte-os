#include <stdarg.h>
#include <time.h>

#include <sys/sem.h>

#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define MEMSIZE 512
#define WORKER_DAY_MAX 2

#define C_RED "\x1b[31m"
#define C_GREEN "\x1b[32m"
#define C_YELLOW "\x1b[33m"
#define C_BLUE "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN "\x1b[36m"
#define C_RESET "\x1b[0m"

pid_t pid_a;
pid_t pid_b;

int main(int argc, char* argv[]);
void handler(int signum);

int leader();
int player(int num);

// used for signals with information
/*union sigval
{
    int sival_int;
    void* sival_ptr;
};*/
