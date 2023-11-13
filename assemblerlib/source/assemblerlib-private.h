#ifndef ASSEMBLERLIB_PRIVATE_H
#define ASSEMBLERLIB_PRIVATE_H

#include <commonlib.h>

enum
{
	// SIC instructions

	OP_ADD = 0x18,
	OP_AND = 0x40,
	OP_COMP = 0x28,
	OP_DIV = 0x24,
	OP_J = 0x3C,
	OP_JEQ = 0x30,
	OP_JGT = 0x34,
	OP_JLT = 0x38,
	OP_JSUB = 0x48,
	OP_LDA = 0x00,
	OP_LDCH = 0x50,
	OP_LDL = 0x08,
	OP_LDX = 0x04,
	OP_MUL = 0x20,
	OP_OR = 0x44,
	OP_RD = 0xD8,
	OP_RSUB = 0x4C,
	OP_STA = 0x0C,
	OP_STCH = 0x54,
	OP_STL = 0x14,
	OP_STSW = 0xE8,
	OP_STX = 0x10,
	OP_SUB = 0x1C,
	OP_TD = 0xE0,
	OP_TIX = 0x2C,
	OP_WD = 0xDC,

	// SIC-XE instructions

	OP_ADDF = 0x58,
	OP_ADDR = 0x90,
	OP_CLEAR = 0xB4,
	OP_COMPF = 0x88,
	OP_COMPR = 0xA0,
	OP_DIVF = 0x64,
	OP_DIVR = 0x9C,
	OP_FIX = 0xC4,
	OP_FLOAT = 0xC0,
	OP_HIO = 0xF4,
	OP_LDB = 0x68,
	OP_LDF = 0x70,
	OP_LDS = 0x6C,
	OP_LDT = 0x74,
	OP_LPS = 0xD0,
	OP_MULF = 0x60,
	OP_MULR = 0x98,
	OP_NORM = 0xC8,
	OP_RMO = 0xAC,
	OP_SHIFTL = 0xA4,
	OP_SHIFTR = 0xA8,
	OP_SIO = 0xF0,
	OP_SSK = 0xEC,
	OP_STB = 0x78,
	OP_STF = 0x80,
	OP_STI = 0xD4,
	OP_STS = 0x7C,
	OP_STT = 0x84,
	OP_SUBF = 0x5C,
	OP_SUBR = 0x94,
	OP_SVC = 0xB0,
	OP_TIO = 0xF8,
	OP_TIXR = 0xB8
};

struct operation_table_entry
{
	unsigned int  opcode;
	unsigned char format;
	enum opty
	{
		OPTY_SIC,
		OPTY_XE,
		OPTY_DIR,
		OPTY_DIR_START,
		OPTY_DIR_END,
		OPTY_DIR_BYTE,
		OPTY_DIR_WORD,
		OPTY_DIR_RESB,
		OPTY_DIR_RESW,
		OPTY_DIR_BASE,
		OPTY_DIR_UNBASE
	} type;
};

enum flags
{
	FLAG_N,
	FLAG_I,
	FLAG_X,
	FLAG_B,
	FLAG_P,
	FLAG_E,
	FLAG_COUNT
};

struct str
{
	const char* start;
	size_t      length;
};

struct symbol_table_entry
{
	unsigned int line_number;
	int          value;
};

struct line_info
{
	struct operation_table_entry operation;
	struct str                   operand;
	bool                         flags[FLAG_COUNT];
	const char*                  error;
};

struct assembler_state
{
	struct map operation_table;
	struct map symbol_table;
	struct array line_infos;
	unsigned int location_counter;
	// Make sure to add 1 when printing.
	unsigned int current_line;
};

struct assembler_state assembler_state_new(void);
void assembler_state_del(struct assembler_state* state);

void parse_file(struct assembler_state* state, struct str source);
void parse_line(struct assembler_state* state, struct str line, unsigned int linenum);
void parse_label(struct assembler_state* state, struct str label, unsigned int linenum);
void parse_operation(struct assembler_state* state, struct str operation, unsigned int linenum);
void parse_operand(struct assembler_state* state, struct str operand, unsigned int linenum);

/*-------------------------------
  Number systems conversion API
-------------------------------*/

char* reverse(char* str);

/* To binary conversions. */
extern char* hexadecimal_to_binary(const char* hexadecimal);
extern char* decimal_to_binary(const int* decimal);

/* To hexadecimal coversions. */
extern char* binary_to_hexadecimal(const char* binary);
extern char* decimal_to_hexadecimal(const int* decimal);

/* To decimal conversions. */
extern int binary_to_decimal(const char* binary);
extern int hexadecimal_to_decimal(const char* hexadecimal);

#endif//ASSEMBLERLIB_PRIVATE_H
