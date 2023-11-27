#include "assemblerlib-private.h"

#include <limits.h>

static void dir_start(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) = state->program_start = parse_operand_2(state, info->operand);
	memcpy(state->program_name, info->label.start, info->label.length < 6 ? info->label.length : 6);
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
	info->operation.format = 5;
	unsigned int tmp = parse_operand_2(state, info->operand);

	unsigned int len = 0;
	while (tmp > 0)
	{
		tmp /= 2;
		len += 1;
	}

	info->operation.opcode = (len + 7) / 8;

	*assembler_state_location_counter(state) += info->operation.opcode;
}

static void dir_word(struct assembler_state* state, struct line_info* info)
{
	info->operation.format = 6;
	info->operation.opcode = parse_operand_2(state, info->operand);

	*assembler_state_location_counter(state) += 3;
}

static void dir_resb(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) += parse_operand_2(state, info->operand);
}

static void dir_resw(struct assembler_state* state, struct line_info* info)
{
	*assembler_state_location_counter(state) += 3 * parse_operand_2(state, info->operand);
}

static void dir_base(struct assembler_state* state, struct line_info* info)
{
	info->base = parse_operand_2(state, info->operand);
}

static void dir_unbase(struct assembler_state* state, struct line_info* info)
{
	info->base = -1;
}

static void dir_use(struct assembler_state* state, struct line_info* info)
{
	if (info->operand.start)
	{
		unsigned int* prev_location = map_getn(&state->location_counters, info->operand.start, info->operand.length);
		const unsigned int location = prev_location ? *prev_location : 0;
		map_setn(&state->location_counters, info->operand.start, info->operand.length, &location);
		state->current_block.start  = info->operand.start;
		state->current_block.length = info->operand.length;

		info->location = location;
	}
	else
	{
		state->current_block.start  = DEFAULT_BLOCK_NAME;
		state->current_block.length = DEFAULT_BLOCK_LENGTH;
	}
}

static void dir_ltorg(struct assembler_state* state, struct line_info* info)
{
	place_literals(state);
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

static void dir_equ(struct assembler_state* state, struct line_info* info)
{
	struct symbol_table_entry entry =
	{
		.line_number = UINT_MAX,
		.value = parse_operand_2(state, info->operand)
	};
	map_setn(&state->symbol_table, info->label.start, info->label.length, &entry);
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
const directive_func DIR_EQU    = dir_equ;