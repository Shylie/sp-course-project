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

struct array assemble(const char** sources)
{
	struct array states = array_new(sizeof(struct assembler_state), 0);

	for (unsigned int index = 0; sources[index]; index++)
	{
		struct str file_contents = read_file(sources[index]);

		if (file_contents.start)
		{
			struct assembler_state state = assembler_state_new();
			array_append(&states, &state);

			parse_file(array_at(&states, array_size(&states) - 1), file_contents);
		}

		cl_realloc(&file_contents, 0);
	}

	return states;
}