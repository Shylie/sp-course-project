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
	char* file = (argc == 2) ? argv[1] : "literals.txt";

	char* c = assemble(file);
	if (c)
	{
		printf("%s\n", c);

		FILE* output_file = fopen("out.txt", "wb");

		fprintf(output_file, "%s", c);

		fclose(output_file);

		free(c);
	}

	getchar();

	return (c ? EXIT_SUCCESS : EXIT_FAILURE);
}
