#include "assemblerlib-private.h"

static void dir_start(struct assembler_state* state, struct line_info* info)
{
	state->program_start = state->location_counter;
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

}

static void dir_resw(struct assembler_state* state, struct line_info* info)
{
}

static void dir_base(struct assembler_state* state, struct line_info* info)
{
}

static void dir_unbase(struct assembler_state* state, struct line_info* info)
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