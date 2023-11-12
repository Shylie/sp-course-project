#include "assemblerlib-private.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t linelen(const char* s)
{
	const size_t max = strlen(s);
	for (size_t current = 0; current < max; current++)
		if (s[current] == '\n')
			return (size_t)(current);

	return max;
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

static struct array split_line(const char* line, size_t linelen)
{
	struct array lines = array_new(sizeof(struct str), 0);

	const char* tokstart;
	const char* current = line;
	const char* const end = line + linelen;

	while (current < end) {
		while (current < end && iswhitespace(*current))
			current++;

		tokstart = current;

		while (current < end && !iswhitespace(*current))
			current++;

		if (current < end) {
			struct str s = { .start = tokstart, .length = (current - tokstart) };
			array_append(&lines, &s);
		}
	}

	return lines;
}

void parse_file(struct assembler_state* state, const char* source)
{
	/*
		TO-DOs:

		Trim comments (use '.' as delim).
		handle incrementing the current line, locctr, etc.
	*/

	FILE *input;
	input = fopen(source, 'r');

	if (input == NULL) {
		perror("Can't open the specified file.");
		exit(1);
	}
	else {
		char buffer[128];

		/* Begin reading the file line-by-line. */
		while (fgets(buffer, sizeof(buffer), input)) {
			/* Trim the comment from the line if one is present. */
			char *comment_start_pointer = strchr(buffer, '.');
			char *uncommented_line = NULL;

			if (comment_start_pointer) {
				unsigned int position = comment_start_pointer - buffer;

				uncommented_line = malloc(position + 1);
				memcpy(uncommented_line, buffer, position);
				uncommented_line[position] = '\0';
			}
			else
				uncommented_line = buffer;

			/* Begin the actual parsing. */
			/* ... */

		}
	}

	fclose(input);
}

void parse_line(struct assembler_state* state, struct str line, unsigned int linenum)
{
	struct array lines = split_line(line.start, line.length);

	struct line_info info;
	memset(&info, 0, sizeof(struct line_info));
	array_append(&state->line_infos, &info);

	const size_t sz = array_size(&lines);

	if (sz == 3) {
		parse_label(state, *(struct str*)array_at(&lines, 0), linenum);
		parse_operation(state, *(struct str*)array_at(&lines, 1), linenum);
		parse_operand(state, *(struct str*)array_at(&lines, 2), linenum);
	}
	else if (sz == 2) {
		parse_operation(state, *(struct str*)array_at(&lines, 0), linenum);
		parse_operand(state, *(struct str*)array_at(&lines, 1), linenum);
	}
	else
		((struct line_info*)array_at(&state->line_infos, linenum))->error = "invalid syntax";

	array_del(&lines);
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
			if (operand.start[operand.length - 2] == ',' && operand.start[operand.length - 1] == 'X') {
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

	// calculate these later
	info->flags[FLAG_P] = false;
	info->flags[FLAG_B] = false;

	info->operand = operand;
}