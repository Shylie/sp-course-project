#include "assemblerlib.h"
#include "assemblerlib-private.h"

static void fill_op_entry(const char* key, unsigned int opcode, unsigned char format, enum opty opty)
{
	struct operation_table_entry entry = { .opcode = opcode, .format = format, .type = opty };
	map_set(&OPERATION_TABLE, key, &entry);
}

void al_fill_tables(void)
{
	fill_op_entry("ADD", OP_ADD, 3, OPTY_SIC);
	fill_op_entry("+ADD", OP_ADD, 4, OPTY_XE);
	fill_op_entry("AND", OP_AND, 3, OPTY_SIC);
	fill_op_entry("+AND", OP_AND, 4, OPTY_XE);
	fill_op_entry("COMP", OP_COMP, 3, OPTY_SIC);
	fill_op_entry("+COMP", OP_COMP, 4, OPTY_XE);
	fill_op_entry("J", OP_J, 3, OPTY_SIC);
	fill_op_entry("+J", OP_J, 4, OPTY_XE);
	fill_op_entry("JEQ", OP_JEQ, 3, OPTY_SIC);
	fill_op_entry("+JEQ", OP_JEQ, 4, OPTY_XE);
	fill_op_entry("JGT", OP_JGT, 3, OPTY_SIC);
	fill_op_entry("+JGT", OP_JGT, 4, OPTY_XE);
	fill_op_entry("JLT", OP_JLT, 3, OPTY_SIC);
	fill_op_entry("+JLT", OP_JLT, 4, OPTY_XE);
	fill_op_entry("JSUB", OP_JSUB, 3, OPTY_SIC);
	fill_op_entry("+JSUB", OP_JSUB, 4, OPTY_XE);
	fill_op_entry("LDA", OP_LDA, 3, OPTY_SIC);
	fill_op_entry("+LDA", OP_LDA, 4, OPTY_XE);
	fill_op_entry("LDCH", OP_LDCH, 3, OPTY_SIC);
	fill_op_entry("+LDCH", OP_LDCH, 4, OPTY_XE);
	fill_op_entry("LDL", OP_LDL, 3, OPTY_SIC);
	fill_op_entry("+LDL", OP_LDL, 4, OPTY_XE);
	fill_op_entry("LDX", OP_LDX, 3, OPTY_SIC);
	fill_op_entry("+LDX", OP_LDX, 4, OPTY_XE);
	fill_op_entry("MUL", OP_MUL, 3, OPTY_SIC);
	fill_op_entry("+MUL", OP_MUL, 4, OPTY_XE);
	fill_op_entry("OR", OP_OR, 3, OPTY_SIC);
	fill_op_entry("+OR", OP_OR, 4, OPTY_XE);
	fill_op_entry("RD", OP_RD, 3, OPTY_SIC);
	fill_op_entry("+RD", OP_RD, 4, OPTY_XE);
	fill_op_entry("RSUB", OP_RSUB, 3, OPTY_SIC);
	fill_op_entry("+RSUB", OP_RSUB, 4, OPTY_XE);
	fill_op_entry("STA", OP_STA, 3, OPTY_SIC);
	fill_op_entry("+STA", OP_STA, 4, OPTY_XE);
	fill_op_entry("STCH", OP_STCH, 3, OPTY_SIC);
	fill_op_entry("+STCH", OP_STCH, 4, OPTY_XE);
	fill_op_entry("STL", OP_STL, 3, OPTY_SIC);
	fill_op_entry("+STL", OP_STL, 4, OPTY_XE);
	fill_op_entry("STSW", OP_STSW, 3, OPTY_SIC);
	fill_op_entry("+STSW", OP_STSW, 4, OPTY_XE);
	fill_op_entry("STX", OP_STX, 3, OPTY_SIC);
	fill_op_entry("+STX", OP_STX, 4, OPTY_XE);
	fill_op_entry("SUB", OP_SUB, 3, OPTY_SIC);
	fill_op_entry("+SUB", OP_SUB, 4, OPTY_XE);
	fill_op_entry("TD", OP_TD, 3, OPTY_SIC);
	fill_op_entry("+TD", OP_TD, 4, OPTY_XE);
	fill_op_entry("TIX", OP_TIX, 3, OPTY_SIC);
	fill_op_entry("+TIX", OP_TIX, 4, OPTY_XE);
	fill_op_entry("WD", OP_WD, 3, OPTY_SIC);
	fill_op_entry("+WD", OP_WD, 4, OPTY_XE);

	fill_op_entry("ADDF", OP_ADDF, 3, OPTY_XE);
	fill_op_entry("+ADDF", OP_ADDF, 4, OPTY_XE);
	fill_op_entry("ADDR", OP_ADDR, 2, OPTY_XE);
	fill_op_entry("CLEAR", OP_CLEAR, 2, OPTY_XE);
	fill_op_entry("COMPF", OP_COMPF, 3, OPTY_XE);
	fill_op_entry("+COMPF", OP_COMPF, 4, OPTY_XE);
	fill_op_entry("DIVF", OP_DIVF, 3, OPTY_XE);
	fill_op_entry("+DIVF", OP_DIVF, 4, OPTY_XE);
	fill_op_entry("DIVR", OP_DIVR, 2, OPTY_XE);
	fill_op_entry("FIX", OP_FIX, 1, OPTY_XE);
	fill_op_entry("FLOAT", OP_FLOAT, 1, OPTY_XE);
	fill_op_entry("HIO", OP_HIO, 1, OPTY_XE);
	fill_op_entry("LDB", OP_LDB, 3, OPTY_XE);
	fill_op_entry("+LDB", OP_LDB, 4, OPTY_XE);
	fill_op_entry("LDF", OP_LDF, 3, OPTY_XE);
	fill_op_entry("+LDF", OP_LDF, 4, OPTY_XE);
	fill_op_entry("LDS", OP_LDS, 3, OPTY_XE);
	fill_op_entry("+LDS", OP_LDS, 4, OPTY_XE);
	fill_op_entry("LDT", OP_LDT, 3, OPTY_XE);
	fill_op_entry("+LDT", OP_LDT, 4, OPTY_XE);
	fill_op_entry("LPS", OP_LPS, 3, OPTY_XE);
	fill_op_entry("+LPS", OP_LPS, 4, OPTY_XE);
	fill_op_entry("MULF", OP_MULF, 3, OPTY_XE);
	fill_op_entry("+MULF", OP_MULF, 4, OPTY_XE);
	fill_op_entry("MULR", OP_MULR, 2, OPTY_XE);
	fill_op_entry("NORM", OP_NORM, 1, OPTY_XE);
	fill_op_entry("RMO", OP_RMO, 2, OPTY_XE);
	fill_op_entry("SHIFTL", OP_SHIFTL, 2, OPTY_XE);
	fill_op_entry("SHIFTR", OP_SHIFTR, 2, OPTY_XE);
	fill_op_entry("SIO", OP_SIO, 1, OPTY_XE);
	fill_op_entry("SSK", OP_SSK, 3, OPTY_XE);
	fill_op_entry("+SSK", OP_SSK, 4, OPTY_XE);
	fill_op_entry("STB", OP_STB, 3, OPTY_XE);
	fill_op_entry("+STB", OP_STB, 4, OPTY_XE);
	fill_op_entry("STF", OP_STF, 3, OPTY_XE);
	fill_op_entry("+STF", OP_STF, 4, OPTY_XE);
	fill_op_entry("STI", OP_STI, 3, OPTY_XE);
	fill_op_entry("+STI", OP_STI, 4, OPTY_XE);
	fill_op_entry("STS", OP_STS, 3, OPTY_XE);
	fill_op_entry("+STS", OP_STS, 4, OPTY_XE);
	fill_op_entry("STT", OP_STT, 3, OPTY_XE);
	fill_op_entry("+STT", OP_STT, 4, OPTY_XE);
	fill_op_entry("SUBF", OP_SUBF, 3, OPTY_XE);
	fill_op_entry("+SUBF", OP_SUBF, 4, OPTY_XE);
	fill_op_entry("SUBR", OP_SUBR, 2, OPTY_XE);
	fill_op_entry("SVC", OP_SVC, 2, OPTY_XE);
	fill_op_entry("TIO", OP_TIO, 1, OPTY_XE);
	fill_op_entry("TIXR", OP_TIXR, 2, OPTY_XE);

	fill_op_entry("START", 0, 0, OPTY_DIR_START);
	fill_op_entry("END", 0, 0, OPTY_DIR_END);
	fill_op_entry("BYTE", 0, 3, OPTY_DIR_BYTE);
	fill_op_entry("WORD", 0, 3, OPTY_DIR_WORD);
	fill_op_entry("RESB", 1, 0, OPTY_DIR_RESB);
	fill_op_entry("RESW", 1, 0, OPTY_DIR_RESW);
	fill_op_entry("BASE", 0, 0, OPTY_DIR_BASE);
	fill_op_entry("UNBASE", 0, 0, OPTY_DIR_UNBASE);
}

struct map OPERATION_TABLE;
struct map SYMBOL_TABLE;