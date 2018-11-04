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

struct order interpret_line(char *line)
{
	struct order result;
	char *pt;
	pt = strtok(line, ",");
	int col = 0;
	while (pt != NULL)
	{
		//printf("column %d\n", col);
		/*
		*/

		//if (found == 1)
		//{
		switch (col)
		{
		case 0:
			result.id = atoi(pt);
			break;
		case 1:
			//result.name = pt;
			break;
		}
		//}
		//printf("%d.: %s\n", col, pt);

		pt = strtok(NULL, ",");
		col = col + 1;
	}

	return result;
}

struct order *read(int p)
{

	struct order *result = NULL;
	FILE *file = fopen("data.txt", "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	int found = 0;
	while ((read = getline(&line, &len, file)) != -1 && result == NULL)
	{
		//printf("Retrieved line of length %zu :\n", read);
		printf("Line: %s\n", line);
		struct order a = interpret_line(line);
		if (p == a.id)
		{
			result = &a;
		}
		printf("interpreted order: %d\n", a.id);
	}
	return result;
}

int create(struct order *o)
{
	FILE *file = fopen("data.txt", "a");
	fprintf(file, "%d,%s,%s\n", o->id, &o->name, ctime(&o->time));
	fclose(file);
}

int main()
{
	struct order data[100];
	int c = 0;

	struct order example;
	example.id = 0;
	strcpy(example.name, "NAAAAME");
	example.time = time(NULL);

	// create(&example);

	read(0);

	read(1);

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
