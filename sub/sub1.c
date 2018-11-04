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
			if (filter_obj->name[0] != '\0' && strcmp(filter_obj->name, a.name) != 0)
			{
				matches_by_name = 0;
			}

			if (matches_by_id != 1 || matches_by_name != 1)
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
	fprintf(file, "%d,%s,%s\n", o->id, &o->name, ctime(&o->time));
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
			// asdasda
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

					fprintf(fptr2, "%d,%s,%s", a.id, a.name, ctime(&a.time));
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
	printf("{id: %d, name: %s, email: %s, phone: %s, perf: %s, time: %s}\n", o->id, &o->name, &o->email, &o->phone, &o->perf, ctime(&o->time));
}

struct order *read_order()
{
	struct order *result = malloc(sizeof(struct order));
	result->id = -1;
	printf("name:");
	scanf("%s", result->name);
	printf("email:");
	scanf("%s", result->email);
	printf("phone:");
	scanf("%s", result->phone);
	printf("perf:");
	scanf("%s", result->perf);
	result->time = time(NULL);
	return result;
}

int main()
{

	// CREATE - Write to file
	// create(&example);

	// READ - With filter
	/*
	struct order *filter_obj = read_order();
	//filter_obj.id = -1;
	//strcpy(filter_obj.name, "numa");

	int c = -1;
	int *count = &c;
	struct order *result = filter(filter_obj, count, -1);
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
	
*/

	// DELETE
	/*
	struct order filter_obj;
	filter_obj.id = -1;
	strcpy(filter_obj.name, "numa");
	delete (&filter_obj);
*/
	// UPDATE
	/*
	struct order filter_obj;
	filter_obj.id = -1;
	strcpy(filter_obj.name, "numa");

	struct order update_obj;
	update_obj.id = -1;
	strcpy(update_obj.name, "hello");

	update(&filter_obj, &update_obj);
*/

	char menu_option;
	do
	{
		printf("0. List (All)\n");
		printf("1. Create\n");
		printf("2. Read (Filter)\n");
		printf("3. Update (Filter) (Replacement)\n");
		printf("4. Delete (Filter)\n");
		printf("q. Exit\n");
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
			break;
		case '2':
			printf("2 - READ MODE\n");
			struct order *_read_filter = read_order();
			// print_order(_read_filter);

			int _read_c = -1;
			int *_read_count = &_read_c;
			struct order *_read_result = filter(_read_filter, _read_count, -1);
			int _read_i = 0;
			while (_read_i < *_read_count)
			{
				struct order *_read_r = &_read_result[_i];
				print_order(_read_r);
				_read_i++;
			};

			break;
		case 'q':
			printf("Quit\n");
			break;
		}
		printf("\nYour choice is: %c\n", menu_option);
	} while (menu_option != 'q');

	return 0;
}
