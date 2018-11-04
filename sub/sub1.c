#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))

struct order
{
	int id;
	char name[255];
	time_t time;
};

int list(struct order *data, int size)
{
	return 0;
}

int read(struct order *o)
{

	printf("time is: %s", ctime(&o->time));
	return 0;
}

int create(struct order *o)
{
	printf("Creating: time is: %s", ctime(&o->time));

	FILE *file = fopen("data.txt", "a");
	// id, name, time
	char *formatted = sprintf("%d,%s,%s\n", o->id, &o->name, ctime(&o->time));

	fprintf(file, formatted);
}

int main()
{
	struct order data[100];
	int c = 0;

	struct order example;
	example.id = 0;
	strcpy(example.name, "NAAAAME");
	example.time = time(NULL);

	FILE *file;
	file = fopen("data.txt", "w+");

	create(&example);

	/*
	printf("size of: %d", LENGTH(data));
	printf("current %d", c);
	append(data, &example, &c, LENGTH(data));
	printf("current %d", c);
	read(&example);
	read(&data[0]);

	char menu_option;
	do
	{
		printf("0. List\n");
		printf("1. Create\n");
		printf("2. Read\n");
		printf("3. Update\n");
		printf("4. Delete\n");
		printf("q. Exit\n");
		scanf(" %c", &menu_option);
		switch (menu_option)
		{
		case '0':
			printf("0\n");
			break;
		case '1':
			printf("1\n");
			break;
		case '2':
			printf("2\n");
			break;
		case 'q':
			printf("Quit\n");
			break;
		}
		printf("\nYour choice is: %c\n", menu_option);
	} while (menu_option != 'q');
*/
	return 0;
}
