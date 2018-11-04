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

// array pointer and the size of that
struct arrs
{
	int pt;
	int s;
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
			strcpy(result.name, pt);
			break;
		}
		//}
		//printf("%d.: %s\n", col, pt);

		pt = strtok(NULL, ",");
		col = col + 1;
	}

	return result;
}

struct order *filter(struct order *filter_obj, int *size, int here)
{
	if (filter_obj == NULL)
	{
		printf("You ahve no filter, list all");
	}
	else
	{
		printf("Your filter: %d, %s\n", filter_obj->id, filter_obj->name);
	}

	FILE *file = fopen("data.txt", "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	struct order *res = NULL;

	if (here > 0)
	{
		res = malloc(here * sizeof(struct order));

		printf("MALLOC HAPPENED!!!!here:  %d size: %d\n", here, *size);
	}

	*size = here;
	printf("size ASSIGNED!!!!here:  %d size: %d\n", here, *size);

	int found = 0;
	while ((read = getline(&line, &len, file)) != -1)
	{
		//printf("Retrieved line of length %zu :\n", read);
		printf("Line: %s", line);
		struct order a = interpret_line(line);
		int match = 1;
		if (filter_obj != NULL)
		{

			int matches_by_id = 1;
			if (filter_obj->id >= 0 && filter_obj->id == a.id)
			{
				matches_by_id = 0;
				printf("no match by id\n");
			}
			int matches_by_name = 1;
			if (filter_obj->name[0] != '\0' && strcmp(filter_obj->name, a.name) != 0)
			{
				matches_by_name = 0;
				printf("no match by name, compared: %s, and %s \n", filter_obj->name, a.name);
			}

			if (matches_by_id != 1 || matches_by_name != 1)
			{
				match = 0;
			}
		}
		if (match == 1)
		{
			printf("Found order %d\n", found);

			if (res != NULL)
			{
				printf("Found order WRITE TO ARRAY: %d\n", a.id);
				res[found] = a;
			}

			found = found + 1;
		}
	}
	fclose(file);

	if (res == NULL) // I know the size, go for filter
	{
		printf("I know the size, go for filter\n");
		return filter(filter_obj, size, found);
	}
	else // I know the result, go for return
	{
		printf("I know the result, go for return\n");
		return res;
	}
}

int create(struct order *o)
{
	FILE *file = fopen("data.txt", "a");
	fprintf(file, "%d,%s,%s\n", o->id, &o->name, ctime(&o->time));
	fclose(file);
}

int delete_line(int lno)
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
			ctr++;
			if (ctr != lno)
			{
				fprintf(fptr2, "%s", str);
			}
		}
	}
	fclose(fptr1);
	fclose(fptr2);
	remove("data.txt");
	rename(temp, "data.txt");
}

int main()
{
	struct order data[100];

	struct order example;
	example.id = 0;
	strcpy(example.name, "NAAAAME");
	example.time = time(NULL);

	// CREATE - Write to file
	// create(&example);

	// READ - With filter
	/*
	struct order filter_obj;
	filter_obj.id = -1;
	strcpy(filter_obj.name, "numa");
	int c = -1;
	int *count = &c;
	struct order *result = filter(&filter_obj, count, -1);
	printf("RETURNED RES SIZE FOR FILTER: %d\n", *count);
	int i = 0;
	while (i < *count)
	{
		printf("READ THE FILTERED RESULT %d\n", i);
		struct order *r = &result[i];
		printf("RETURNED RES: %d\n", r->id);
		i++;
	}
*/
	// READ - Without filter (read all)
	/*
	printf("READ - Without filter (read all)\n");

	int _c = -1;
	int *_count = &_c;
	printf("READ \n");
	struct order *_result = filter(NULL, _count, -1);
	printf("RETURNED RES SIZE FOR FILTER: %d\n", *_count);
	int _i = 0;
	while (_i < *_count)
	{
		printf("READ THE FILTERED RESULT %d\n", _i);
		struct order *_r = &_result[_i];
		printf("RETURNED RES: id: %d name: %s\n", _r->id, _r->name);
		_i++;
	};
*/

	// DELETE

	delete_line(1);

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
