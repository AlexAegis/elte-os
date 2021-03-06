#include <stdarg.h>
#include <time.h>

#include <sys/sem.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define MEMSIZE 1024

char *order_format = "{id: %d, name: %s, email: %s, phone: %s, perf: %s, time: %s}\n";
// gcc -lpthread sub2.c
pid_t pid;
int *s; // semaphore array
sem_t *van_meg;
sem_t *sem_write_guard;
sem_t *sem_read_guard;
//
sem_t *szemafor_letrehozas(char *nev, int szemafor_ertek)
{
    sem_t *semid = sem_open(nev, O_CREAT, S_IRUSR | S_IWUSR, szemafor_ertek);
    if (semid == SEM_FAILED)
        perror("sem_open");
    return semid;
}

int sem_lock(sem_t *sem, char *name)
{
    printf("%i \t- locking ", pid);
    printf("%s", name);
    printf(" semaphore\n");
    sem_wait(sem);
    printf("%i \t- locked! ", pid);
    printf("%s", name);
    printf(" semaphore\n");
}
int sem_release(sem_t *sem, char *name)
{
    printf("%i \t- releasing ", pid);
    printf("%s", name);
    printf(" semaphore\n");
    sem_post(sem);
    printf("%i \t- released! ", pid);
    printf("%s", name);
    printf(" semaphore\n");
}

void szemafor_torles(char *nev)
{
    sem_unlink(nev);
}

struct order
{
    int id;
    char name[255];
    char email[255];
    char phone[255];
    char perf[255];
    time_t time;
};

struct order interpret_line(char *line)
{
    struct order result;
    char *pt;
    pt = strtok(line, ",");
    int col = 0;
    while (pt != NULL)
    {
        switch (col)
        {
        case 0:
            result.id = atoi(pt);
            break;
        case 1:
            strcpy(result.name, pt);
            break;
        case 2:
            strcpy(result.email, pt);
            break;
        case 3:
            strcpy(result.phone, pt);
            break;
        case 4:
            strcpy(result.perf, pt);
            break;
        case 5:
            result.time = time(NULL);
            break;
        }
        pt = strtok(NULL, ",");
        col = col + 1;
    }

    return result;
}

struct order *filter(struct order *filter_obj, int *size, int here)
{
    FILE *file = fopen("data.txt", "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    //printf("Filter. size: %d, here: %d", *size, here);
    struct order *res = NULL;
    //printf("Hejhó\n");
    if (here > 0)
    {
        //printf("MALLOCCED\n");
        res = malloc(here * sizeof(struct order));
    }
    //printf("1\n");
    *size = here;
    int found = 0;

    //printf("seedddsg!\n");
    while ((read = getline(&line, &len, file)) != -1)
    {
        //printf("getline!\n");
        struct order a = interpret_line(line);
        int match = 1;
        if (filter_obj != NULL)
        {

            int matches_by_id = 1;
            if (filter_obj->id >= 0 && filter_obj->id == a.id)

            {
                matches_by_id = 0;
            }
            int matches_by_name = 1;
            if (filter_obj->name[0] != '\0' && !strstr(a.name, filter_obj->name))
            {
                matches_by_name = 0;
            }
            int matches_by_email = 1;
            if (filter_obj->email[0] != '\0' && !strstr(a.email, filter_obj->email))
            {
                matches_by_email = 0;
            }
            int matches_by_phone = 1;
            if (filter_obj->phone[0] != '\0' && !strstr(a.phone, filter_obj->phone))
            {
                matches_by_phone = 0;
            }
            int matches_by_perf = 1;
            if (filter_obj->perf[0] != '\0' && !strstr(a.perf, filter_obj->perf))
            {
                matches_by_perf = 0;
            }

            if (matches_by_id != 1 || matches_by_name != 1 || matches_by_email != 1 || matches_by_phone != 1 || matches_by_perf != 1)
            {
                match = 0;
            }
        }
        if (match == 1)
        {
            if (res != NULL)
            {
                res[found] = a;
            }

            found = found + 1;
        }
    }
    fclose(file);

    if (res == NULL && found > 0) // I know the size, go for filter
    {
        //printf("I know the size, go for filter, size: %d, found: %d", size, found);
        return filter(filter_obj, size, found);
    }
    else // I know the result, go for return
    {
        //printf("I know the result, go for return");
        return res;
    }
}

int create(struct order *o)
{
    FILE *file = fopen("data.txt", "a");
    fprintf(file, "%d,%s,%s,%s,%s,%s}\n", o->id, &o->name, &o->email, &o->phone, &o->perf, ctime(&o->time));
    fclose(file);
}

int update(struct order *filter_obj, struct order *update_obj)
{
    int ctr = 0;
    char ch;
    FILE *fptr1, *fptr2;
    char str[255], temp[] = "temp.txt";

    fptr1 = fopen("data.txt", "r");
    if (!fptr1)
    {
        return 0;
    }
    fptr2 = fopen(temp, "w");
    if (!fptr2)
    {
        fclose(fptr1);
        return 0;
    }
    while (!feof(fptr1))
    {
        strcpy(str, "\0");
        fgets(str, 255, fptr1);
        if (!feof(fptr1))
        {
            char line[255];
            strcpy(line, str);

            struct order a = interpret_line(str);
            int match = 1;
            if (filter_obj != NULL)
            {

                int matches_by_id = 1;
                if (filter_obj->id >= 0 && filter_obj->id == a.id)
                {
                    matches_by_id = 0;
                }
                int matches_by_name = 1;
                if (filter_obj->name[0] != '\0' && strcmp(filter_obj->name, a.name) != 0)
                {
                    matches_by_name = 0;
                }

                if (matches_by_id != 1 || matches_by_name != 1)
                {
                    match = 0;
                }
            }
            ctr++;
            if (match != 1)
            {
                fprintf(fptr2, "%s", line);
            }
            else
            {

                if (update_obj != NULL)
                {

                    if (update_obj->id >= 0)
                    {
                        a.id = update_obj->id;
                    }
                    if (update_obj->name[0] != '\0')
                    {
                        a.name[0] = update_obj->name[0];
                    }
                    if (update_obj->email[0] != '\0')
                    {
                        a.email[0] = update_obj->email[0];
                    }
                    if (update_obj->phone[0] != '\0')
                    {
                        a.phone[0] = update_obj->phone[0];
                    }
                    if (update_obj->perf[0] != '\0')
                    {
                        a.perf[0] = update_obj->perf[0];
                    }
                    fprintf(fptr2, "%d,%s,%s,%s,%s,%s}\n", a.id, a.name, a.email, a.phone, a.perf, ctime(&a.time));
                }
            }
        }
    }
    fclose(fptr1);
    fclose(fptr2);
    remove("data.txt");
    rename(temp, "data.txt");
}

int delete (struct order *filter_obj)
{
    return update(filter_obj, NULL);
}

void print_order(struct order *o)
{
    printf("%i \t- ", pid);
    printf(order_format, o->id, &o->name, &o->email, &o->phone, &o->perf, ctime(&o->time));
}

char *str_order(struct order *o)
{
    char buf[200];
}

struct order *read_order()
{
    char buffer[255];
    struct order *result = malloc(sizeof(struct order));
    result->id = -1;
    printf("Enter an order. ('-' to ignore field):\n");
    printf("name:");
    scanf("%s", buffer);
    if (strcmp(buffer, "-") != 0)
    {
        strcpy(result->name, buffer);
    }

    printf("email:");
    scanf("%s", buffer);
    if (strcmp(buffer, "-") != 0)
    {
        strcpy(result->email, buffer);
    }
    printf("phone:");
    scanf("%s", buffer);
    if (strcmp(buffer, "-") != 0)
    {
        strcpy(result->phone, buffer);
    }
    printf("perf:");
    scanf("%s", buffer);
    if (strcmp(buffer, "-") != 0)
    {
        strcpy(result->perf, buffer);
    }
    result->time = time(NULL);
    return result;
}

void print_sem()
{
    printf("%i \t- s[0]: %i, s[1]: %i\n", pid, s[0], s[1]);
}

int main(int argc, char *argv[])
{
    printf("main start\n");

    int pipefd[2]; // unnamed pipe file descriptor array

    char buffer[100]; // char array for reading from pipe

    // SEMAPHORE STUFF  - START
    key_t kulcs;
    int sh_mem_id;
    char *sem_name_van_meg = "van_meg";
    char *sem_name_sem_write_guard = "sem_write_guard";
    char *sem_name_sem_read_guard = "sem_read_guard";

    //
    kulcs = ftok(argv[0], 1);
    sh_mem_id = shmget(kulcs, MEMSIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    s = (int *)shmat(sh_mem_id, NULL, 0);
    s[0] = 0; // a polcon levo kenyerek szama
    // s[1] = 1; // mehet a folyamat
    van_meg = szemafor_letrehozas(sem_name_van_meg, 0);
    sem_write_guard = szemafor_letrehozas(sem_name_sem_write_guard, 0);
    sem_read_guard = szemafor_letrehozas(sem_name_sem_read_guard, 0);
    // SEMAPHORE STUFF  - END

    if (pipe(pipefd) == -1)
    {
        perror("Hiba a pipe nyitaskor!\n");
        exit(EXIT_FAILURE);
    }
    pid = fork(); // creating parent-child processes
    if (pid == -1)
    {
        perror("Fork hiba\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) // Szerelő (Child)
    {
        sleep(1);

        sem_lock(van_meg, sem_name_van_meg);
        int count = s[0];
        printf("%i \t- count: %i\n", pid, count);
        print_sem();
        sem_release(van_meg, sem_name_van_meg);
        int i = 0;
        while (i < count)
        {

            sem_lock(sem_read_guard, sem_name_sem_read_guard);

            printf("%i \t- szerelő indul: \n", pid);
            //sleep(1); // sleeping a few seconds, not necessary

            close(pipefd[1]); //Usually we close the unused write end

            printf("%i \t- Gyerek elkezdi olvasni a csobol az adatokat!\n", pid);

            struct order *order_buffer;
            read(pipefd[0], order_buffer, 4096 /*sizeof(struct order)*/); // reading max 100 chars
            printf("%i \t- olvasta uzenet: %i\n", pid, order_buffer->id);

            print_order(order_buffer);
            close(pipefd[0]); // finally we close the used read end

            sem_release(sem_write_guard, sem_name_sem_write_guard);

            i = i + 1;
        }
    }
    else // Társaság (Parent) process fájlkezelés csak itt lehet
    {
        sem_release(van_meg, sem_name_van_meg);
        sem_lock(van_meg, sem_name_van_meg);
        printf("%i \t- start társaság!\n", pid);
        int _c = -1;
        int *_count = &_c;
        struct order *_result = filter(NULL, _count, -1);
        printf("%i \t- result count of all: %i\n", pid, *_count);
        int _i = 0;
        s[0] = *_count; // darabszám
        int current_count = s[0];
        if (*_count > 0)
        {
            s[1] = 1; // még van
        }
        else
        {
            s[1] = 0; // eleve nem is volt
        }
        print_sem();
        sem_release(van_meg, sem_name_van_meg);
        sem_release(sem_write_guard, sem_name_sem_write_guard);
        while (_i < 1) // *_count
        {

            sem_lock(sem_write_guard, sem_name_sem_write_guard);

            struct order *_r = &_result[_i];
            printf("%i \t- feladat fájlból kiolvasva: \n", pid);
            print_order(_r);

            printf("%i \t- Társaság - sizeof order: %i\n", pid, 4096);

            close(pipefd[0]); //Usually we close unused read end
            write(pipefd[1], _r, sizeof(_r));
            close(pipefd[1]); // Closing write descriptor
            printf("%i \t- Szulo beirta az adatokat a csobe!\n", pid);

            sem_release(sem_read_guard, sem_name_sem_read_guard);
            _i++;
        };
        printf("%i \t- Társaság - locking van_meg semaphore to set none left\n", pid);
        sem_wait(van_meg);
        s[1] = 0; // már nincs
        print_sem();
        sem_post(van_meg);
        printf("%i \t- Társaság - released van_meg semaphore to set none left\n", pid);

        fflush(NULL); // flushes all write buffers (not necessary)
                      //wait();		  // waiting for child process (not necessary)
                      // try it without wait()

        wait(NULL);
        shmdt(s);
        shmctl(sh_mem_id, IPC_RMID, NULL);
        szemafor_torles(sem_name_van_meg);
        szemafor_torles(sem_name_sem_write_guard);
        szemafor_torles(sem_name_sem_read_guard);

        printf("Szulo befejezte!\n");
    }

    exit(EXIT_SUCCESS); // force exit, not necessary

    return 0;
}
