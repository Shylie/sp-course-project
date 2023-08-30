#ifndef ASSEMBLERLIB_PRIVATE_H
#define ASSEMBLERLIB_PRIVATE_H

// ORDER OF SOURCE PROCESSING
// 1. source code
// 2. token list
// 3. operation list
// 4. machine code

#include <stdbool.h>

typedef struct
{
	enum
	{
		// SIC instructions

		OP_ADD    = 0x18,
		OP_AND    = 0x40,
		OP_COMP   = 0x28,
		OP_DIV    = 0x24,
		OP_J      = 0x3C,
		OP_JEQ    = 0x30,
		OP_JGT    = 0x34,
		OP_JLT    = 0x38,
		OP_JSUB   = 0x48,
		OP_LDA    = 0x00,
		OP_LDCH   = 0x50,
		OP_LDL    = 0x08,
		OP_LDX    = 0x04,
		OP_MUL    = 0x20,
		OP_OR     = 0x44,
		OP_RD     = 0xD8,
		OP_RSUB   = 0x4C,
		OP_STA    = 0x0C,
		OP_STCH   = 0x54,
		OP_STL    = 0x14,
		OP_STSW   = 0xE8,
		OP_STX    = 0x10,
		OP_SUB    = 0x1C,
		OP_TD     = 0xE0,
		OP_TIX    = 0x2C,
		OP_WD     = 0xDC,

		// SIC-XE instructions

		OP_ADDF   = 0x58,
		OP_ADDR   = 0x90,
		OP_CLEAR  = 0xB4,
		OP_COMPF  = 0x88,
		OP_COMPR  = 0xA0,
		OP_DIVF   = 0x64,
		OP_DIVR   = 0x9C,
		OP_FIX    = 0xC4,
		OP_FLOAT  = 0xC0,
		OP_HIO    = 0xF4,
		OP_LDB    = 0x68,
		OP_LDF    = 0x70,
		OP_LDS    = 0x6C,
		OP_LDT    = 0x74,
		OP_LPS    = 0xD0,
		OP_MULF   = 0x60,
		OP_MULR   = 0x98,
		OP_NORM   = 0xC8,
		OP_RMO    = 0xAC,
		OP_SHIFTL = 0xA4,
		OP_SHIFTR = 0xA8,
		OP_SIO    = 0xF0,
		OP_SSK    = 0xEC,
		OP_STB    = 0x78,
		OP_STF    = 0x80,
		OP_STI    = 0xD4,
		OP_STS    = 0x7C,
		OP_STT    = 0x84,
		OP_SUBF   = 0x5C,
		OP_SUBR   = 0x94,
		OP_SVC    = 0xB0,
		OP_TIO    = 0xF8,
		OP_TIXR   = 0xB8
	} code; 

	union
	{
		struct
		{
			unsigned char register1;
			unsigned char register2;
		} fmt2;

		struct
		{
			enum
			{
				FLAG_NONE = 0,
				FLAG_N = 1 << 0,
				FLAG_I = 1 << 1,
				FLAG_X = 1 << 2,
				FLAG_B = 1 << 3,
				FLAG_P = 1 << 4,
				FLAG_E = 1 << 5
			} flags;

			unsigned int disp; // also used to represent address for format 4 instructions
		} fmt34;
	} as;
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
} symbol_table;

typedef struct
{
	const char*  name;
	unsigned int opcode;
} opcode_table_entry;

typedef struct
{
	enum
	{
		TOKEN_START,
		TOKEN_END,
		TOKEN_BYTE,
		TOKEN_WORD,
		TOKEN_RESB,
		TOKEN_RESW,
		TOKEN_LABEL,
		TOKEN_OPERATION,
		TOKEN_CONSTANT
	} id;

	const char*  lexeme;
	unsigned int lexeme_length;
} token;

typedef struct
{
	const char* start;
	const char* current;
} tokenizer;

enum
{
	NUM_OPCODE_TABLE_ENTRIES = 59
};
const extern opcode_table_entry OPCODE_TABLE[NUM_OPCODE_TABLE_ENTRIES];

bool is_alpha(char c);

int lookup_opcode(const char* text);

char tokenizer_next(tokenizer* t);
char tokenizer_peek(tokenizer* t);
token tokenizer_next_token(tokenizer* t);

#endif//ASSEMBLERLIB_PRIVATE_H