#include <errno.h> //perror, errno
#include <fcntl.h> //open,creat
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen,strcmp,strcat,strcpy and more ....
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h> //open
#include <sys/types.h>
#include <sys/wait.h> //waitpid
#include <time.h>
#include <unistd.h> //fork, pipe()
#include <wait.h>
struct tour
{
    int count;
    char passengerList[15][50];
    int price;
};

void sigtermHandler(sigNumber)
{
    printf("SIGTERM handler\n");
}
void sigusr1Handler(sigNumber)
{
    printf("SIGUSR1 handler\n");
}

int main(int argc, char* argv[])
{
    signal(SIGTERM, sigtermHandler);
    signal(SIGUSR1, sigusr1Handler);

    pid_t pid;
    int parentToChild[2];
    int childToParent[2];

    if (pipe(parentToChild) == -1)
    {
        perror("Hiba a parentToChild nyitasakor");
        exit(EXIT_FAILURE);
    }

    if (pipe(childToParent) == -1)
    {
        perror("Hiba a childToParent nyitasakor");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork hiba");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));

    if (pid != 0)
    {
        //felelossegteljes szulo/
        close(parentToChild[0]); //Usually we close unused read end
        close(childToParent[1]); //Usually we close unused write end

        struct tour newTour;
        int count = rand() % (15 + 1 - 5) + 5;
        int price = rand() % (100 + 1 - 60) + 60;

        newTour.count = count;
        newTour.price = price;

        int i;
        for (i = 0; i < count; ++i)
        {
            stpcpy(newTour.passengerList[i], "utas");
        }

        pause();

        write(parentToChild[1], &newTour, sizeof(newTour));

        int ar, sordb = 0;
        read(childToParent[0], &sordb, sizeof(sordb));
        read(childToParent[0], &ar, sizeof(ar));
        printf("SKB: sor = %d, ar = %d\n", sordb, ar);
        pause();

        int status;
        wait(&status); // waiting for child process (not necessary)*/
    }
    else
    {
        //undok, rosszcsont gyerek/

        close(parentToChild[1]); //Usually we close unused write end
        close(childToParent[0]); //Usually we close unused read end

        pid_t parent = getppid();
        struct tour newTour;

        sleep(1);

        kill(parent, SIGUSR1);

        read(parentToChild[0], &newTour, sizeof(newTour));
        int count = newTour.count;
        int i;
        int ar = 0, sordb = 0;
        for (i = 0; i < count; ++i)
        {
            printf(" SBD> resztvevo(%d) %s\n", i, newTour.passengerList[i]);
            ar = ar + newTour.price;
            sordb = sordb + 5;
        }
        printf("SBD :sor = %d, ar = %d \n", sordb, ar);

        write(childToParent[1], &sordb, sizeof(sordb));
        write(childToParent[1], &ar, sizeof(ar));

        int sleepValue = rand() % (5 + 1 - 1) + 1;
        printf("SBD> sleep(%d)\n", sleepValue);
        sleep(sleepValue);
        printf("SBD -> SKB: kesz (SIGTERM)\n");
        kill(parent, SIGTERM);
    }
    return 0;
}