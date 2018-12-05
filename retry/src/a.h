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

char* order_format = "{id: %i, name: %s, email: %s, phone: %s, perf: %i, time: %s, done: %i}\n";

pid_t pid;

struct order* all_orders; // only for the dispatch simulation

int main(int argc, char* argv[]);
void handler(int signum);

int worker();
int company();
int dispatch_logic(int* current_task);

// used for signals with information
/*union sigval
{
    int sival_int;
    void* sival_ptr;
};*/

struct order
{
    int id;
    char name[255];
    char email[255];
    char phone[255];
    int perf;
    time_t time;
    int done;
};
void print_order(struct order* o)
{
    printf("%i \t- ", pid);
    printf(order_format, o->id, &o->name, &o->email, &o->phone, o->perf, ctime(&o->time), o->done);
}
struct order interpret_line(char* line)
{
    struct order result;
    char* pt;
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
            result.perf = atoi(pt);
            break;
        case 5:
            result.time = time(NULL); // TODO: Time reading not implemented
            break;
        case 6:
            result.done = atoi(pt);
            break;
        }
        pt = strtok(NULL, ",");
        col = col + 1;
    }

    return result;
}

struct order* filter(struct order* filter_obj, int* size, int here)
{
    FILE* file = fopen("src/data.txt", "r");
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    //printf("Filter. size: %d, here: %d", *size, here);
    struct order* res = NULL;
    //printf("Hejhó\n");
    if (here > 0)
    {
        //printf("MALLOCCED\n");
        res = malloc(here * sizeof(struct order));
    }
    //printf("1\n");
    *size = here;
    int found = 0;

    // printf("seedddsg!\n");
    while ((read = getline(&line, &len, file)) != -1)
    {
        //printf("getline!\n");
        struct order a = interpret_line(line);
        int match = 1;
        if (filter_obj != NULL)
        {

            int matches_by_id = 1;
            if (filter_obj->id >= 0 && filter_obj->id != a.id)
            {
                //printf("matches_by_id = 0;!\n");
                matches_by_id = 0;
            }
            int matches_by_name = 1;
            if (filter_obj->name[0] != '\0' && !strstr(a.name, filter_obj->name))
            {
                //printf("matches_by_name = 0;!\n");
                matches_by_name = 0;
            }
            int matches_by_email = 1;
            if (filter_obj->email[0] != '\0' && !strstr(a.email, filter_obj->email))
            {
                // printf("matches_by_email = 0;!\n");
                matches_by_email = 0;
            }
            int matches_by_phone = 1;
            if (filter_obj->phone[0] != '\0' && !strstr(a.phone, filter_obj->phone))
            {
                //printf("matches_by_phone = 0;!\n");
                matches_by_phone = 0;
            }
            int matches_by_perf = 1;
            if (filter_obj->perf >= 0 && filter_obj->perf != a.perf)
            {
                //printf("matches_by_perf = 0;!\n");
                matches_by_perf = 0;
            }
            int matches_by_done = 1;
            if (filter_obj->done >= 0 && filter_obj->done != a.done)
            {
                //printf("matches_by_done = 0;!\n");
                matches_by_done = 0;
            }

            if (matches_by_id != 1 || matches_by_name != 1 || matches_by_email != 1 || matches_by_phone != 1 || matches_by_perf != 1 || matches_by_done != 1)
            {
                //printf("match = 0;!\n");
                match = 0;
            }
        }
        if (match == 1)
        {
            if (res != NULL)
            {
                res[found] = a;
            }
            //printf(C_RED "I FOUND SOMETHING, ILL WRITE THIS TWICE!" C_RESET "\n");
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

int create(struct order* o)
{
    FILE* file = fopen("data.txt", "a");
    fprintf(file, "%i,%s,%s,%s,%i,%s,%i}\n", o->id, o->name, o->email, o->phone, o->perf, ctime(&o->time), o->done);
    fclose(file);
}

int update(struct order* filter_obj, struct order* update_obj)
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
                    if (update_obj->perf >= 0)
                    {
                        a.perf = update_obj->perf;
                    }
                    if (update_obj->done >= 0)
                    {
                        a.done = update_obj->done;
                    }
                    fprintf(fptr2, "%d,%s,%s,%s,%d,%s,%d}\n", a.id, a.name, a.email, a.phone, a.perf, ctime(&a.time), a.done);
                }
            }
        }
    }
    fclose(fptr1);
    fclose(fptr2);
    remove("data.txt");
    rename(temp, "data.txt");
}

int delete (struct order* filter_obj)
{
    return update(filter_obj, NULL);
}

char* str_order(struct order* o)
{
    char buf[200];
}

struct order* read_order()
{
    char buffer[255];
    struct order* result = malloc(sizeof(struct order));
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
        result->perf = atoi(buffer);
    }
    result->time = time(NULL);
    return result;
}
