#include "assemblerlib.h"
#include "assemblerlib-private.h"

#include <string.h>

int lookup_opcode(const char* text)
{
	for (int i = 0; i < NUM_OPCODE_TABLE_ENTRIES; i++)
	{
		if (strcmp(text, OPCODE_TABLE[i].name) == 0)
		{
			return OPCODE_TABLE[i].opcode;
		}
	}

	return -1;
}

bool is_alpha(char c)
{
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

char tokenizer_next(tokenizer* t)
{
	return *t->current++;
}

char tokenizer_peek(tokenizer* t)
{
	return *t->current;
}

token tokenizer_next_token(tokenizer* t)
{
}

al_machine_code assemble(const char* source)
{
	// do it
}