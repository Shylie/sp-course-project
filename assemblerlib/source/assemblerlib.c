#include "assemblerlib.h"
#include "assemblerlib-private.h"

#include <stdio.h>

static struct str read_file(const char* fname)
{
	FILE* fp = fopen(fname, "rb");
	if (!fp)
	{
		return (struct str){ .start = NULL, .length = 0 };
	}

	fseek(fp, 0, SEEK_END);
	const long size = ftell(fp);
	rewind(fp);

	char* buffer = NULL;
	cl_realloc(&buffer, size);
	
	fread(buffer, size, 1, fp);
	fclose(fp);

	return (struct str) { .start = buffer, .length = size };
}

char* assemble(const char* source)
{
	struct str file_contents = read_file(source);

	char* c = NULL;

	if (file_contents.start)
	{
		struct assembler_state state = assembler_state_new();
		c = parse_file(&state, file_contents);
		assembler_state_del(&state);
	}

	cl_realloc(&file_contents, 0);

	return c;
}