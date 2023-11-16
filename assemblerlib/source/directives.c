#include "assemblerlib-private.h"

static void dir_start(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) = 0; // set to actual value
	memcpy(&state->program_name, info->operand.start, info->operand.length < 6 ? info->operand.length : 6);
}

static void dir_end(struct assembler_state* state, struct line_info* info)
{
	struct symbol_table_entry* entry = map_getn(&state->symbol_table, info->operand.start, info->operand.length);
	if (entry)
	{
		state->program_first_instruction = entry->value;
	}
	else
	{
		state->program_first_instruction = state->program_start;
	}
}

static void dir_byte(struct assembler_state* state, struct line_info* info)
{
}

static void dir_word(struct assembler_state* state, struct line_info* info)
{
}

static void dir_resb(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) += parse_operand_2(info->operand);
}

static void dir_resw(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) += 3 * parse_operand_2(info->operand);
}

static void dir_base(struct assembler_state* state, struct line_info* info)
{
}

static void dir_unbase(struct assembler_state* state, struct line_info* info)
{
}

static void dir_use(struct assembler_state* state, struct line_info* info)
{
}

static void dir_ltorg(struct assembler_state* state, struct line_info* info)
{
}

static void dir_macro(struct assembler_state* state, struct line_info* info)
{
}

static void dir_mend(struct assembler_state* state, struct line_info* info)
{
}

static void dir_extdef(struct assembler_state* state, struct line_info* info)
{
}

static void dir_extref(struct assembler_state* state, struct line_info* info)
{
}

const directive_func DIR_START  = dir_start;
const directive_func DIR_END    = dir_end;
const directive_func DIR_BYTE   = dir_byte;
const directive_func DIR_WORD   = dir_word;
const directive_func DIR_RESB   = dir_resb;
const directive_func DIR_RESW   = dir_resw;
const directive_func DIR_BASE   = dir_base;
const directive_func DIR_UNBASE = dir_unbase;
const directive_func DIR_USE    = dir_use;
const directive_func DIR_LTORG  = dir_ltorg;
const directive_func DIR_MACRO  = dir_macro;
const directive_func DIR_MEND   = dir_mend;
const directive_func DIR_EXTDEF = dir_extdef;
const directive_func DIR_EXTREF = dir_extref;

const char* const  DEFAULT_BLOCK_NAME   = "  ";
const unsigned int DEFAULT_BLOCK_LENGTH = 2;