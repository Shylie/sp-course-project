#ifndef ASSEMBLERLIB_PRIVATE_H
#define ASSEMBLERLIB_PRIVATE_H

#include <stdbool.h>

enum
{
	FLAG_NONE = 0,
	FLAG_N = 1 << 5,
	FLAG_I = 1 << 4,
	FLAG_X = 1 << 3,
	FLAG_B = 1 << 2,
	FLAG_P = 1 << 1,
	FLAG_E = 1 << 0
};

typedef struct
{
	unsigned int code;
	unsigned int flags; // also r1 for format 2
	unsigned int disp; // also r2 for format 2, and address for format 4
} operation;

typedef struct
{
	const char*  label;
	unsigned int value;
} symbol_table_entry;

typedef struct
{
	symbol_table_entry* entries;
	unsigned int        table_size;
	unsigned int        max_table_size;
} symbol_table;

typedef struct
{
	const char*  name;
	unsigned int opcode;
	unsigned char size;
} opcode_table_entry;

typedef struct
{
	const char*  name;
} keyword_table_entry;

const extern opcode_table_entry OPCODE_TABLE[];
const extern int OPCODE_TABLE_SIZE;
const extern keyword_table_entry KEYWORD_TABLE[];
const extern int KEYWORD_TABLE_SIZE;

symbol_table symbol_table_create();
void symbol_table_delete(symbol_table* symtab);
void symbol_table_add(symbol_table* symtab, symbol_table_entry entry);
int symbol_table_search(symbol_table* symtab, const char* symbol);
int symbol_table_search_n(symbol_table* symtab, const char* symbol, unsigned int n);

bool is_alpha(char c);
bool is_number(char c);

int lookup_opcode(const char* text);
int lookup_opcode_n(const char* text, unsigned int n);

int lookup_keyword(const char* text);
int lookup_keyword_n(const char* text, unsigned int n);

#endif//ASSEMBLERLIB_PRIVATE_H
