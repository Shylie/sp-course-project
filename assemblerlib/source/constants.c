#include "assemblerlib.h"
#include "assemblerlib-private.h"

static void fill_op_entry_full(struct assembler_state* state, const char* key, unsigned int opcode, unsigned char format, enum opty opty, directive_func func)
{
	struct operation_table_entry entry = { .opcode = opcode, .format = format, .type = opty, .directive = func };
	map_set(&state->operation_table, key, &entry);
}

static void fill_op_entry(struct assembler_state* state, const char* key, unsigned int opcode, unsigned char format, enum opty opty)
{
	fill_op_entry_full(state, key, opcode, format, opty, NULL);
}

struct assembler_state assembler_state_new(void)
{
	struct assembler_state state =
	{
		.operation_table = map_new(sizeof(struct operation_table_entry)),
		.symbol_table = map_new(sizeof(struct symbol_table_entry)),
		.line_infos = array_new(sizeof(struct line_info), 16),
		.location_counter = 0
	};

	fill_op_entry(&state, "ADD", OP_ADD, 3, OPTY_SIC);
	fill_op_entry(&state, "+ADD", OP_ADD, 4, OPTY_XE);
	fill_op_entry(&state, "AND", OP_AND, 3, OPTY_SIC);
	fill_op_entry(&state, "+AND", OP_AND, 4, OPTY_XE);
	fill_op_entry(&state, "COMP", OP_COMP, 3, OPTY_SIC);
	fill_op_entry(&state, "+COMP", OP_COMP, 4, OPTY_XE);
	fill_op_entry(&state, "J", OP_J, 3, OPTY_SIC);
	fill_op_entry(&state, "+J", OP_J, 4, OPTY_XE);
	fill_op_entry(&state, "JEQ", OP_JEQ, 3, OPTY_SIC);
	fill_op_entry(&state, "+JEQ", OP_JEQ, 4, OPTY_XE);
	fill_op_entry(&state, "JGT", OP_JGT, 3, OPTY_SIC);
	fill_op_entry(&state, "+JGT", OP_JGT, 4, OPTY_XE);
	fill_op_entry(&state, "JLT", OP_JLT, 3, OPTY_SIC);
	fill_op_entry(&state, "+JLT", OP_JLT, 4, OPTY_XE);
	fill_op_entry(&state, "JSUB", OP_JSUB, 3, OPTY_SIC);
	fill_op_entry(&state, "+JSUB", OP_JSUB, 4, OPTY_XE);
	fill_op_entry(&state, "LDA", OP_LDA, 3, OPTY_SIC);
	fill_op_entry(&state, "+LDA", OP_LDA, 4, OPTY_XE);
	fill_op_entry(&state, "LDCH", OP_LDCH, 3, OPTY_SIC);
	fill_op_entry(&state, "+LDCH", OP_LDCH, 4, OPTY_XE);
	fill_op_entry(&state, "LDL", OP_LDL, 3, OPTY_SIC);
	fill_op_entry(&state, "+LDL", OP_LDL, 4, OPTY_XE);
	fill_op_entry(&state, "LDX", OP_LDX, 3, OPTY_SIC);
	fill_op_entry(&state, "+LDX", OP_LDX, 4, OPTY_XE);
	fill_op_entry(&state, "MUL", OP_MUL, 3, OPTY_SIC);
	fill_op_entry(&state, "+MUL", OP_MUL, 4, OPTY_XE);
	fill_op_entry(&state, "OR", OP_OR, 3, OPTY_SIC);
	fill_op_entry(&state, "+OR", OP_OR, 4, OPTY_XE);
	fill_op_entry(&state, "RD", OP_RD, 3, OPTY_SIC);
	fill_op_entry(&state, "+RD", OP_RD, 4, OPTY_XE);
	fill_op_entry(&state, "RSUB", OP_RSUB, 3, OPTY_SIC);
	fill_op_entry(&state, "+RSUB", OP_RSUB, 4, OPTY_XE);
	fill_op_entry(&state, "STA", OP_STA, 3, OPTY_SIC);
	fill_op_entry(&state, "+STA", OP_STA, 4, OPTY_XE);
	fill_op_entry(&state, "STCH", OP_STCH, 3, OPTY_SIC);
	fill_op_entry(&state, "+STCH", OP_STCH, 4, OPTY_XE);
	fill_op_entry(&state, "STL", OP_STL, 3, OPTY_SIC);
	fill_op_entry(&state, "+STL", OP_STL, 4, OPTY_XE);
	fill_op_entry(&state, "STSW", OP_STSW, 3, OPTY_SIC);
	fill_op_entry(&state, "+STSW", OP_STSW, 4, OPTY_XE);
	fill_op_entry(&state, "STX", OP_STX, 3, OPTY_SIC);
	fill_op_entry(&state, "+STX", OP_STX, 4, OPTY_XE);
	fill_op_entry(&state, "SUB", OP_SUB, 3, OPTY_SIC);
	fill_op_entry(&state, "+SUB", OP_SUB, 4, OPTY_XE);
	fill_op_entry(&state, "TD", OP_TD, 3, OPTY_SIC);
	fill_op_entry(&state, "+TD", OP_TD, 4, OPTY_XE);
	fill_op_entry(&state, "TIX", OP_TIX, 3, OPTY_SIC);
	fill_op_entry(&state, "+TIX", OP_TIX, 4, OPTY_XE);
	fill_op_entry(&state, "WD", OP_WD, 3, OPTY_SIC);
	fill_op_entry(&state, "+WD", OP_WD, 4, OPTY_XE);

	fill_op_entry(&state, "ADDF", OP_ADDF, 3, OPTY_XE);
	fill_op_entry(&state, "+ADDF", OP_ADDF, 4, OPTY_XE);
	fill_op_entry(&state, "ADDR", OP_ADDR, 2, OPTY_XE);
	fill_op_entry(&state, "CLEAR", OP_CLEAR, 2, OPTY_XE);
	fill_op_entry(&state, "COMPF", OP_COMPF, 3, OPTY_XE);
	fill_op_entry(&state, "+COMPF", OP_COMPF, 4, OPTY_XE);
	fill_op_entry(&state, "COMPR", OP_COMPR, 2, OPTY_XE);
	fill_op_entry(&state, "DIVF", OP_DIVF, 3, OPTY_XE);
	fill_op_entry(&state, "+DIVF", OP_DIVF, 4, OPTY_XE);
	fill_op_entry(&state, "DIVR", OP_DIVR, 2, OPTY_XE);
	fill_op_entry(&state, "FIX", OP_FIX, 1, OPTY_XE);
	fill_op_entry(&state, "FLOAT", OP_FLOAT, 1, OPTY_XE);
	fill_op_entry(&state, "HIO", OP_HIO, 1, OPTY_XE);
	fill_op_entry(&state, "LDB", OP_LDB, 3, OPTY_XE);
	fill_op_entry(&state, "+LDB", OP_LDB, 4, OPTY_XE);
	fill_op_entry(&state, "LDF", OP_LDF, 3, OPTY_XE);
	fill_op_entry(&state, "+LDF", OP_LDF, 4, OPTY_XE);
	fill_op_entry(&state, "LDS", OP_LDS, 3, OPTY_XE);
	fill_op_entry(&state, "+LDS", OP_LDS, 4, OPTY_XE);
	fill_op_entry(&state, "LDT", OP_LDT, 3, OPTY_XE);
	fill_op_entry(&state, "+LDT", OP_LDT, 4, OPTY_XE);
	fill_op_entry(&state, "LPS", OP_LPS, 3, OPTY_XE);
	fill_op_entry(&state, "+LPS", OP_LPS, 4, OPTY_XE);
	fill_op_entry(&state, "MULF", OP_MULF, 3, OPTY_XE);
	fill_op_entry(&state, "+MULF", OP_MULF, 4, OPTY_XE);
	fill_op_entry(&state, "MULR", OP_MULR, 2, OPTY_XE);
	fill_op_entry(&state, "NORM", OP_NORM, 1, OPTY_XE);
	fill_op_entry(&state, "RMO", OP_RMO, 2, OPTY_XE);
	fill_op_entry(&state, "SHIFTL", OP_SHIFTL, 2, OPTY_XE);
	fill_op_entry(&state, "SHIFTR", OP_SHIFTR, 2, OPTY_XE);
	fill_op_entry(&state, "SIO", OP_SIO, 1, OPTY_XE);
	fill_op_entry(&state, "SSK", OP_SSK, 3, OPTY_XE);
	fill_op_entry(&state, "+SSK", OP_SSK, 4, OPTY_XE);
	fill_op_entry(&state, "STB", OP_STB, 3, OPTY_XE);
	fill_op_entry(&state, "+STB", OP_STB, 4, OPTY_XE);
	fill_op_entry(&state, "STF", OP_STF, 3, OPTY_XE);
	fill_op_entry(&state, "+STF", OP_STF, 4, OPTY_XE);
	fill_op_entry(&state, "STI", OP_STI, 3, OPTY_XE);
	fill_op_entry(&state, "+STI", OP_STI, 4, OPTY_XE);
	fill_op_entry(&state, "STS", OP_STS, 3, OPTY_XE);
	fill_op_entry(&state, "+STS", OP_STS, 4, OPTY_XE);
	fill_op_entry(&state, "STT", OP_STT, 3, OPTY_XE);
	fill_op_entry(&state, "+STT", OP_STT, 4, OPTY_XE);
	fill_op_entry(&state, "SUBF", OP_SUBF, 3, OPTY_XE);
	fill_op_entry(&state, "+SUBF", OP_SUBF, 4, OPTY_XE);
	fill_op_entry(&state, "SUBR", OP_SUBR, 2, OPTY_XE);
	fill_op_entry(&state, "SVC", OP_SVC, 2, OPTY_XE);
	fill_op_entry(&state, "TIO", OP_TIO, 1, OPTY_XE);
	fill_op_entry(&state, "TIXR", OP_TIXR, 2, OPTY_XE);

	fill_op_entry_full(&state, "START", 0, 0, OPTY_DIR, DIR_START);
	fill_op_entry_full(&state, "END", 0, 0, OPTY_DIR, DIR_END);
	fill_op_entry_full(&state, "BYTE", 0, 0, OPTY_DIR, DIR_BYTE);
	fill_op_entry_full(&state, "WORD", 0, 0, OPTY_DIR, DIR_WORD);
	fill_op_entry_full(&state, "RESB", 0, 0, OPTY_DIR, DIR_RESB);
	fill_op_entry_full(&state, "RESW", 0, 0, OPTY_DIR, DIR_RESW);
	fill_op_entry_full(&state, "BASE", 0, 0, OPTY_DIR, DIR_BASE);
	fill_op_entry_full(&state, "UNBASE", 0, 0, OPTY_DIR, DIR_UNBASE);

	return state;
}

void assembler_state_del(struct assembler_state* state)
{
	map_del(&state->operation_table);
	map_del(&state->symbol_table);
	array_del(&state->line_infos);
}