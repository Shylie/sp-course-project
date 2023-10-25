#include <commonlib.h>

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

	getchar();

	return 0;
}