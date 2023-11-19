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
	char* c = assemble("literals.txt");
	if (c)
	{
		printf("%s\n", c);
		free(c);
	}

	getchar();

	return (c ? EXIT_SUCCESS : EXIT_FAILURE);
}
