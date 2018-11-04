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
			if (filter_obj->name[0] != '\0' && strcmp(filter_obj->name, a.name) != 0)
			{
				matches_by_name = 0;
			}
			int matches_by_email = 1;
			if (filter_obj->email[0] != '\0' && strcmp(filter_obj->email, a.email) != 0)
			{
				matches_by_email = 0;
			}
			int matches_by_phone = 1;
			if (filter_obj->phone[0] != '\0' && strcmp(filter_obj->phone, a.phone) != 0)
			{
				matches_by_phone = 0;
			}
			int matches_by_perf = 1;
			if (filter_obj->perf[0] != '\0' && strcmp(filter_obj->perf, a.perf) != 0)
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
	printf("{id: %d, name: %s, email: %s, phone: %s, perf: %s, time: %s}\n", o->id, &o->name, &o->email, &o->phone, &o->perf, ctime(&o->time));
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

	return 0;
}
