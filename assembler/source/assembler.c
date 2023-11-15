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
	struct array arr = assemble((const char* []) { "basic.txt", NULL });

	array_del(&arr);

	getchar();

	return 0;
}
