#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
char *order_format = "{id: %d, name: %s, email: %s, phone: %s, perf: %s, time: %s}\n";
// gcc -lpthread sub2.c

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

	if (here > 0)
	{
		//printf("MALLOCCED");
		res = malloc(here * sizeof(struct order));
	}
	//printf("1");
	*size = here;
	int found = 0;
	while ((read = getline(&line, &len, file)) != -1)
	{
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
	printf(order_format, o->id, &o->name, &o->email, &o->phone, &o->perf, ctime(&o->time));
}

char *str_order(struct order *o)
{
	char buf[200];
}
void formatstr(const char *format, ...)
{

	//return buffer;
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

int main()
{
	printf("main start\n");

	int pipefd[2]; // unnamed pipe file descriptor array
	pid_t pid;
	char buffer[100]; // char array for reading from pipe
	if (pipe(pipefd) == -1)
	{
		perror("Hiba a pipe nyitaskor!");
		exit(EXIT_FAILURE);
	}
	pid = fork(); // creating parent-child processes
	if (pid == -1)
	{
		perror("Fork hiba");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) // Szerelő
	{
		sleep(1); // sleeping a few seconds, not necessary
		printf("%i", pid);
		close(pipefd[1]); //Usually we close the unused write end
		printf("Gyerek elkezdi olvasni a csobol az adatokat!\n");
		read(pipefd[0], buffer, sizeof(buffer)); // reading max 100 chars
		printf("Gyerek olvasta uzenet: %s", buffer);
		printf("\n");
		close(pipefd[0]); // finally we close the used read end
	}
	else // Társaság (Parent) process fájlkezelés csak itt lehet
	{
		printf("Társaság - start pid: %i!\n", pid);

		int _c = -1;
		int *_count = &_c;
		struct order *_result = filter(NULL, _count, -1);
		int _i = 0;
		while (_i < *_count)
		{
			struct order *_r = &_result[_i];
			printf("Társaság - feladat kiolvasva: ", pid);
			print_order(_r);

			int bufSize = 10;
			char *mystr = "This is my string!";
			char *buf = malloc(bufSize);

			if (snprintf(buf, bufSize, "%s", mystr) >= bufSize)
			{
				bufSize *= 2;
				printf("Not enough space. Trying %d bytes\n", bufSize);
				free(buf);
				buf = malloc(bufSize);

				if (snprintf(buf, bufSize, "%s", mystr) >= bufSize)
				{
					printf("Still not enough space. Aborting\n");
					exit(1);
				}
			}

			printf("There was enough space!\n");
			printf("buf: %s\n", buf);

			/*va_list args;
			va_start(args, format);
			vsprintf(order_buffer, order_format, _r->id, &_r->name, &_r->email, &_r->phone, &_r->perf, ctime(&_r->time));
*/
			close(pipefd[0]); //Usually we close unused read end
			write(pipefd[1], "Hajra Fradi!", 13);
			close(pipefd[1]); // Closing write descriptor

			_i++;
		};

		close(pipefd[0]); //Usually we close unused read end
		write(pipefd[1], "Hajra Fradi!", 13);
		close(pipefd[1]); // Closing write descriptor
		printf("Szulo beirta az adatokat a csobe!\n");
		fflush(NULL); // flushes all write buffers (not necessary)
		wait();		  // waiting for child process (not necessary)
					  // try it without wait()
		printf("Szulo befejezte!");
	}
	exit(EXIT_SUCCESS); // force exit, not necessary

	/*
	char menu_option;
	do
	{
		printf("\n\n--Mészáros és Mészáros Benzines Napelem Kft.-- \n\n Jellemzően egy évre vonatkoztatva 1000KWh energiát 1KW teljesítményt nyújtó 4 darab napelem panel ad.\n\n");
		printf("0. List (All)\n");
		printf("1. Create\n");
		printf("2. Read (Filter)\n");
		printf("3. Update (Filter) (Replacement)\n");
		printf("4. Delete (Filter)\n");
		printf("q. Exit\n");
		printf("\nYour input: ");
		scanf(" %c", &menu_option);
		switch (menu_option)
		{
		case '0':
			printf("0 - LIST MODE\n");
			int _c = -1;
			int *_count = &_c;
			struct order *_result = filter(NULL, _count, -1);
			int _i = 0;
			while (_i < *_count)
			{
				struct order *_r = &_result[_i];
				print_order(_r);
				_i++;
			};
			break;
		case '1':
			printf("1 - CREATE MODE\n");
			printf("enter order data\n");
			create(read_order());
			break;
		case '2':
			printf("2 - READ MODE\n");
			printf("enter read filter\n");
			struct order *_read_filter = read_order();
			// print_order(_read_filter);

			int _read_c = -1;
			int *_read_count = &_read_c;
			struct order *_read_result = filter(_read_filter, _read_count, -1);
			int _read_i = 0;
			while (_read_i < *_read_count)
			{
				struct order *_read_r = &_read_result[_read_i];
				print_order(_read_r);
				_read_i++;
			};

			break;
		case '3':
			printf("3 - UPDATE MODE\n");
			printf("enter update filter\n");
			struct order *_update_filter = read_order();
			printf("enter replace\n");
			struct order *_update_replace = read_order();

			update(_update_filter, _update_replace);

			break;
		case '4':
			printf("4 - DELETE MODE\n");
			printf("enter delete filter\n");
			struct order *_delete_filter = read_order();

			delete (_delete_filter);

			break;
		case 'q':
			printf("Quit\n");
			break;
		}
	} while (menu_option != 'q');
	*/
	return 0;
}
