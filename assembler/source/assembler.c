#include <commonlib.h>
#include <assemblerlib.h>

#include <stdio.h>

static void each(int* element, size_t element_size, void* ud)
{
	printf("%d\n", *element);
}

struct data
{
	int a;
	float b;
};

int main(int argc, char** argv)
{
	struct array arr = array_new(sizeof(int), 0);

	int v;

	v = 10;
	array_append(&arr, &v);

	v = 15;
	array_append(&arr, &v);

	v = 25;
	array_append(&arr, &v);

	v = 40;
	array_append(&arr, &v);

	v = 69;
	array_append(&arr, &v);

	printf(
		"%d, %d [%d, %d, %d, %d, %d]\n",
		array_size(&arr),
		array_max_size(&arr),
		*((int*)array_at(&arr, 0)),
		*((int*)array_at(&arr, 1)),
		*((int*)array_at(&arr, 2)),
		*((int*)array_at(&arr, 3)),
		*((int*)array_at(&arr, 4))
	);

	array_remove(&arr, 1);
	printf(
		"%d, %d [%d, %d, %d, %d]\n",
		array_size(&arr),
		array_max_size(&arr),
		*((int*)array_at(&arr, 0)),
		*((int*)array_at(&arr, 1)),
		*((int*)array_at(&arr, 2)),
		*((int*)array_at(&arr, 3))
	);

	v = 25;
	int* found = array_find(&arr, &v);
	if (found)
	{
		printf("found: %d\n", *found);
	}
	else
	{
		printf("not found\n");
	}

	v = 182305;
	found = array_find(&arr, &v);
	if (found)
	{
		printf("found: %d\n", *found);
	}
	else
	{
		printf("not found\n");
	}

	printf("begin foreach\n");
	array_foreach(&arr, each, NULL);
	printf("end foreach\n");

	array_del(&arr);

	printf("array test done\n");

	struct map m = map_new(sizeof(int));
	found = map_get(&m, "gamer", strlen("gamer"));
	
	if (found)
	{
		printf("found: %d\n", *found);
	}
	else
	{
		printf("not found\n");
	}

	v = 69420;
	map_set(&m, "gamer", strlen("gamer"), &v);
	printf("set\n");

	found = map_get(&m, "gamer", strlen("gamer"));
	if (found)
	{
		printf("found: %d\n", *found);
	}
	else
	{
		printf("not found\n");
	}

	float v2 = 3.4f;
	struct map m2 = map_new(sizeof(float));

	float* found2 = map_get(&m2, "gamer", strlen("gamer"));

	if (found2)
	{
		printf("found: %f\n", *found2);
	}
	else
	{
		printf("not found\n");
	}

	map_set(&m2, "gamer", strlen("gamer"), &v2);
	found2 = map_get(&m2, "gamer", strlen("gamer"));

	if (found2)
	{
		printf("found: %f\n", *found2);
	}
	else
	{
		printf("not found\n");
	}

	struct map m3 = map_new(sizeof(struct data));

	struct data* found3 = map_get(&m3, "gamer", strlen("gamer"));

	if (found3)
	{
		printf("found: %d %f\n", found3->a, found3->b);
	}
	else
	{
		printf("not found\n");
	}

	struct data d = { .a = 69, .b = 4.20f };
	map_set(&m3, "gamer", strlen("gamer"), &d);

	found3 = map_get(&m3, "gamer", strlen("gamer"));

	if (found3)
	{
		printf("found: %d %f\n", found3->a, found3->b);
	}
	else
	{
		printf("not found\n");
	}

	map_del(&m);
	map_del(&m2);
	map_del(&m3);


	/* Conversions test. */

	int choice;
	char input[100]; // Buffer for user input 

	while (1) {
		printf("\nMenu:\n");
		printf("1. Hexadecimal to Binary (Works.)\n");
		printf("2. Decimal to Binary (Works.)\n\n");
		printf("3. Binary to Hexadecimal (Needs to be fixed.)\n");
		printf("4. Decimal to Hexadecimal (Not complete.)\n\n");
		printf("5. Binary to Decimal (Works.)\n");
		printf("6. Hexadecimal to Decimal (Not complete.)\n\n");
		printf("7. Exit\n\n");
		printf("Enter your choice: ");

		scanf("%d", &choice);

		if (choice == 7)
			break;

		switch (choice) {
		case 1:
			scanf("%s", input);
			char *result1 = hexadecimal_to_binary(input);
			printf("%s\n", result1);
      free(result1);
			break;
		case 2:
			scanf("%d", &choice);
			char *result2 = decimal_to_binary(&choice);
			printf("%s\n", result2);
      free(result2);
			break;
		case 3:
			scanf("%s", input);
			char *result3 = binary_to_hexadecimal(input);
			printf("%s\n", result3);
      free(result3);
			break;
		case 4:
			scanf("%d", &choice);
			char *result4 = decimal_to_hexadecimal(&choice);
			printf("%s\n", result4);
      free(result4);
			break;
		case 5:
			scanf("%s", input);
			int result5 = binary_to_decimal(input);
			printf("%d\n", result5);
			break;
		case 6:
			scanf("%s", input);
			int result6 = hexadecimal_to_decimal(input);
			printf("%d\n", result6);
			break;
		default:

		}
	}



	return 0;
}
