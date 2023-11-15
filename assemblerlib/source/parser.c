#include "assemblerlib-private.h"

#include <stdlib.h>
#include <string.h>

static struct array split_file(struct str file)
{
	struct array lines = array_new(sizeof(struct str), 0);

	const char* linestart = file.start;
	const char* comment = NULL;

	for (const char* current = file.start; current < file.start + file.length;)
	{
		switch (*current++)
		{
		case '\n':
		{
			struct str line = { .start = linestart, .length = comment ? (comment - linestart) : (current - linestart) };
			array_append(&lines, &line);
			linestart = current;
			comment = NULL;
			break;
		}

		case '.':
			if (!comment)
			{
				comment = current;
			}
			break;
		}
	}

	return lines;
}

static bool iswhitespace(const char c)
{
	switch (c) {
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return true;
		default:
			return false;
	}
}

static struct array split_line(struct str line)
{
	struct array tokens = array_new(sizeof(struct str), 0);

	const char* tokstart;
	const char* current = line.start;
	const char* const end = line.start + line.length;

	while (current < end) {
		while (current < end && iswhitespace(*current))
			current++;

		tokstart = current;

		while (current < end && !iswhitespace(*current))
			current++;

		if (current < end) {
			struct str s = { .start = tokstart, .length = (current - tokstart) };
			array_append(&tokens, &s);
		}
	}

	return tokens;
}

void parse_file(struct assembler_state* state, struct str source)
{
	struct array lines = split_file(source);

	for (unsigned int i = 0; i < array_size(&lines); i++)
	{
		parse_line(state, *(struct str*)array_at(&lines, i), i);

		struct line_info* info = array_at(&state->line_infos, i);

		info->location = state->location_counter;

		if (info->operation.type == OPTY_DIR)
		{
			info->operation.directive(state, info);
		}
		else
		{
			state->location_counter += info->operation.format;
		}
	}

	state->program_length = state->location_counter;

	array_del(&lines);
}

void parse_line(struct assembler_state* state, struct str line, unsigned int linenum)
{
	struct array tokens = split_line(line);

	struct line_info info;
	memset(&info, 0, sizeof(struct line_info));
	array_append(&state->line_infos, &info);

	const size_t sz = array_size(&tokens);

	if (sz == 3) {
		parse_label(state, *(struct str*)array_at(&tokens, 0), linenum);
		parse_operation(state, *(struct str*)array_at(&tokens, 1), linenum);
		parse_operand(state, *(struct str*)array_at(&tokens, 2), linenum);
	}
	else if (sz == 2) {
		parse_operation(state, *(struct str*)array_at(&tokens, 0), linenum);
		parse_operand(state, *(struct str*)array_at(&tokens, 1), linenum);
	}
	else
		((struct line_info*)array_at(&state->line_infos, linenum))->error = "invalid syntax";

	array_del(&tokens);
}

void parse_label(struct assembler_state* state, struct str label, unsigned int linenum)
{
	if (map_getn(&state->symbol_table, label.start, label.length))
		((struct line_info*)array_at(&state->line_infos, linenum))->error = "multiple defitions for label";
	else {
		struct symbol_table_entry entry = { .line_number = linenum, .value = state->location_counter };
		map_setn(&state->symbol_table, label.start, label.length, &entry);
	}
}

void parse_operation(struct assembler_state* state, struct str operation, unsigned int linenum)
{
	struct operation_table_entry* op = map_getn(&state->operation_table, operation.start, operation.length);

	if (op)
		((struct line_info*)array_at(&state->line_infos, linenum))->operation = *op;
	else
		((struct line_info*)array_at(&state->line_infos, linenum))->error = "unknown operation";
}

void parse_operand(struct assembler_state* state, struct str operand, unsigned int linenum)
{
	struct line_info* info = array_at(&state->line_infos, linenum);

	switch (info->operation.format) {
		case 4:
			info->flags[FLAG_E] = true;
			// fall through here
		case 3:
			if (operand.length > 2 && operand.start[operand.length - 2] == ',' && operand.start[operand.length - 1] == 'X') {
				info->flags[FLAG_N] = true;
				info->flags[FLAG_I] = true;
				info->flags[FLAG_X] = true;
			}
			else if (operand.start[0] == '#') {
				info->flags[FLAG_N] = false;
				info->flags[FLAG_I] = true;
				info->flags[FLAG_X] = false;
			}
			else if (operand.start[0] == '@') {
				info->flags[FLAG_N] = true;
				info->flags[FLAG_I] = false;
				info->flags[FLAG_X] = false;
			}
	}

	// calculate these later during target address calculation
	info->flags[FLAG_P] = false;
	info->flags[FLAG_B] = false;

	info->operand = operand;
}