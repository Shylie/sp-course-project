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

typedef void (*directive_func)(struct assembler_state*, struct line_info* info);

struct operation_table_entry
{
	unsigned int  opcode;
	unsigned char format;
	enum opty
	{
		OPTY_SIC,
		OPTY_XE,
		OPTY_DIR_1,
		OPTY_DIR_2
	} type;
	directive_func directive;
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
	long         value;
};

struct literal_table_entry
{
	unsigned int value;
	long         address;
};

struct line_info
{
	struct str                   line;
	struct str                   label;
	struct operation_table_entry operation;
	struct str                   operand;
	bool                         flags[FLAG_COUNT];
	unsigned int                 location;
	long                         base;
	const char*                  error;
	unsigned int                 line_number;
};

struct assembler_state
{
	struct map operation_table;
	struct map symbol_table;
	struct array line_infos;
	struct str current_block;
	struct map location_counters;
	struct array literals;
	char program_name[6];
	unsigned int program_length;
	unsigned int program_start;
	unsigned int program_first_instruction;
};

struct header_record
{
	char magic_number;
	char program_name[6];
	char program_start[6];
	char program_length[6];
	char newline;
};

struct text_record
{
	char magic_number;
	char object_code_start[6];
	char object_code_length[2];
	char object_code[60];
	char newline;
};

struct end_record
{
	char magic_number;
	char first_instruction[6];
	char newline;
};

struct program
{
	struct header_record header_record;
	struct array         text_records;
	struct end_record    end_record;
};

struct assembler_state assembler_state_new(void);
void assembler_state_del(struct assembler_state* state);

unsigned int* assembler_state_location_counter(struct assembler_state* state);

char* parse_file(struct assembler_state* state, struct str source);
void  parse_line(struct assembler_state* state, struct str line, unsigned int linenum);
void  parse_label(struct assembler_state* state, struct str label, unsigned int linenum);
void  parse_operation(struct assembler_state* state, struct str operation, unsigned int linenum);
void  parse_operand(struct assembler_state* state, struct str operand, unsigned int linenum);
unsigned int parse_operand_2(struct assembler_state* state, struct str operand);
void place_literals(struct assembler_state* state);

unsigned int calculate_target_address(struct line_info* info, long target);
void finish_text_record(struct program* p, unsigned int start, unsigned int end);
struct text_record* current_record(struct program* p);

extern const directive_func DIR_START;
extern const directive_func DIR_END;
extern const directive_func DIR_BYTE;
extern const directive_func DIR_WORD;
extern const directive_func DIR_RESB;
extern const directive_func DIR_RESW;
extern const directive_func DIR_BASE;
extern const directive_func DIR_UNBASE;
extern const directive_func DIR_USE;
extern const directive_func DIR_LTORG;
extern const directive_func DIR_MACRO;
extern const directive_func DIR_MEND;
extern const directive_func DIR_EXTDEF;
extern const directive_func DIR_EXTREF;
extern const directive_func DIR_EQU;

extern const char* const  DEFAULT_BLOCK_NAME;
extern const unsigned int DEFAULT_BLOCK_LENGTH;

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
extern int decimal_to_decimal(struct str decimal);

#endif//ASSEMBLERLIB_PRIVATE_H
