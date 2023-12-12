#include <commonlib.h>
#include <assemblerlib.h>

#include <stdio.h>

int main(int argc, char** argv)
{
	printf("\n");

	char* file = (argc == 2) ? argv[1] : "literals.txt";

	char* c = assemble(file);
	if (c)
	{
		printf("\n%s\n", c);

		FILE* output_file = fopen("out.txt", "wb");

		fprintf(output_file, "%s", c);

		fclose(output_file);

		free(c);
	}

	getchar();

	return (c ? EXIT_SUCCESS : EXIT_FAILURE);
}
