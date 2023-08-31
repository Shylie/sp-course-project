#include "assemblerlib.h"
#include "assemblerlib-private.h"

#include <stdlib.h>
#include <string.h>

symbol_table symbol_table_create()
{
	enum
	{
		INITIAL_SIZE = 8
	};
	return (symbol_table){ .entries = realloc(NULL, INITIAL_SIZE * sizeof(symbol_table_entry)), .table_size = 0, .max_table_size = INITIAL_SIZE };
}

void symbol_table_delete(symbol_table* symtab)
{
	free(symtab->entries);
	symtab->entries = NULL;
	symtab->table_size = 0;
	symtab->max_table_size = 0;
}

void symbol_table_add(symbol_table* symtab, symbol_table_entry entry)
{
	if (symtab->table_size >= symtab->max_table_size)
	{
		symtab->entries = realloc(symtab->entries, 2 * symtab->max_table_size * sizeof(symbol_table_entry));
		symtab->max_table_size *= 2;
	}

	symtab->entries[symtab->table_size++] = entry;
}

int symbol_table_search(symbol_table* symtab, const char* symbol)
{
	for (int i = 0; i < symtab->table_size; i++)
	{
		if (strcmp(symtab->entries[i].label, symbol) == 0)
		{
			return symtab->entries[i].value;
		}
	}

	return -1;
}

int symbol_table_search_n(symbol_table* symtab, const char* symbol, unsigned int n)
{
	for (int i = 0; i < symtab->table_size; i++)
	{
		if (strncmp(symtab->entries[i].label, symbol, n) == 0)
		{
			return symtab->entries[i].value;
		}
	}

	return -1;
}

bool is_alpha(char c)
{
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool is_number(char c)
{
	return c >= '0' && c <= '9';
}

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

int lookup_opcode_n(const char* text, unsigned int n)
{
	for (int i = 0; i < NUM_OPCODE_TABLE_ENTRIES; i++)
	{
		if (strncmp(text, OPCODE_TABLE[i].name, n) == 0)
		{
			return OPCODE_TABLE[i].opcode;
		}
	}

	return -1;
}

int lookup_keyword(const char* text)
{
	for (int i = 0; i < NUM_OPCODE_TABLE_ENTRIES; i++)
	{
		if (strcmp(text, KEYWORD_TABLE[i].name) == 0)
		{
			return KEYWORD_TABLE[i].token_id;
		}
	}

	return -1;
}

int lookup_keyword_n(const char* text, unsigned int n)
{
	for (int i = 0; i < NUM_OPCODE_TABLE_ENTRIES; i++)
	{
		if (strncmp(text, KEYWORD_TABLE[i].name, n) == 0)
		{
			return KEYWORD_TABLE[i].token_id;
		}
	}

	return -1;
}

char tokenizer_next(tokenizer* t)
{
	const unsigned int offset = (unsigned int)(t->current - t->start);
	if (offset >= strlen(t->start))
	{
		return 0;
	}
	else
	{
		return *t->current++;
	}
}

char tokenizer_peek(tokenizer* t)
{
	return tokenizer_peek_n(t, 0);
}

char tokenizer_peek_n(tokenizer* t, unsigned int n)
{
	const unsigned int offset = (unsigned int)(t->current - t->start) + n;
	if (offset >= strlen(t->start))
	{
		return 0;
	}
	else
	{
		return t->current[n];
	}
}

token tokenizer_next_token(tokenizer* t)
{
	const char c = tokenizer_peek(t);
	const char c2 = tokenizer_peek_n(t, 1);
	if ((is_alpha(c)) ||
		(c == '+' && is_alpha(c2)) ||
		(c == '@' && is_alpha(c2)) ||
		(c == '#' && is_alpha(c2)))
	{
		// tokenizer_handle_text
	}
}

al_machine_code assemble(const char* source)
{
	// do it
}