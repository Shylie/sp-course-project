#include "assemblerlib-private.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

enum
{
	MAX_TEXT_RECORD_LENGTH = 60
};

static unsigned int map_max(const char* key, unsigned int* value, unsigned int* ud)
{
	if (*ud < *value)
	{
		*ud = *value;
	}
}

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
			struct str line = { .start = linestart, .length = comment ? (comment - linestart - 1) : (current - linestart) };
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
	switch (c)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		return true;

	default:
		return false;
	}
}

static bool isop(const char c)
{
	switch (c)
	{
	case '+':
	case '-':
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

static struct array split_operand(struct str op)
{
	struct array tokens = array_new(sizeof(struct str), 1);

	const char* tokstart;
	const char* current = op.start;
	const char* const end = op.start + op.length;

	while (current < end)
	{
		if (current < end && isop(*current))
		{
			struct str s =
			{
				.start = current,
				.length = 1
			};
			array_append(&tokens, &s);

			current++;
		}

		tokstart = current;

		while (current < end && !isop(*current)) { current++; }

		if (current - tokstart > 0)
		{
			struct str s =
			{
				.start = tokstart,
				.length = (current - tokstart)
			};

			array_append(&tokens, &s);
		}
	}

	return tokens;
}

char* parse_file(struct assembler_state* state, struct str source)
{
	struct array lines = split_file(source);

	// pass 1
	for (unsigned int i = 0; i < array_size(&lines); i++)
	{
		struct str line = *(struct str*)array_at(&lines, i);
		parse_line(state, line, i);

		struct line_info* info = array_at(&state->line_infos, array_size(&state->line_infos) - 1);

		info->location = *assembler_state_location_counter(state);
		info->line = line;

		if (info->operation.type == OPTY_DIR_1)
		{
			info->operation.directive(state, info);
		}
		else
		{
			*assembler_state_location_counter(state) += info->operation.format;
		}

		printf("%X\t%.*s\n", info->location, line.length, line.start);
	}
	place_literals(state);

	// pass 2
	for (unsigned int i = 0; i < array_size(&state->line_infos); i++)
	{
		struct line_info* info = array_at(&state->line_infos, i);
		{
			struct symbol_table_entry entry =
			{
				.line_number = i,
				.value = info->location
			};
			map_set(&state->symbol_table, "*", &entry);
		}

		if (info->error) { continue; }
		if (i > 0)
		{
			info->base = ((struct line_info*)array_at(&state->line_infos, i - 1))->base;
		}
		else
		{
			info->base = -1;
		}

		if (info->operation.type == OPTY_DIR_2)
		{
			info->operation.directive(state, info);
		}
	}

	// pass 3
	struct program program;
	memset(&program, ' ', sizeof(program));
	program.header_record.magic_number = 'H';
	memcpy(&program.header_record.program_name, &state->program_name, 6);
	{
		char buf[7];
		snprintf(buf, 7, "%.6X", state->program_start);
		memcpy(program.header_record.program_start, buf, 6);

		unsigned int maxloc = state->program_start;
		map_foreach(&state->location_counters, map_max, &maxloc);

		snprintf(buf, 7, "%.6X", maxloc - state->program_start);
		memcpy(program.header_record.program_length, buf, 6);
	}
	program.header_record.newline = '\n';
	program.text_records = array_new(sizeof(struct text_record), 1);
	program.end_record.magic_number = 'E';
	{
		char buf[7];
		snprintf(buf, 7, "%.6X", state->program_first_instruction);
		memcpy(program.end_record.first_instruction, buf, 6);
	}
	program.end_record.newline = '\n';

	{
		struct text_record text;
		memset(&text, '0', sizeof(text));
		memset(&text.object_code, ' ', sizeof(text.object_code));
		text.magic_number = 'T';
		text.newline = '\n';
		array_append(&program.text_records, &text);
	}

	unsigned int text_record_start;
	unsigned char text_record_address = 0;

	for (unsigned int i = 0; i < array_size(&state->line_infos); i++)
	{
		struct line_info* info = array_at(&state->line_infos, i);

		if (i == 0)
		{
			text_record_start = info->location;
		}

		if (info->error || info->operation.format == 0) { continue; }

		switch (info->operation.format)
		{
		case 1:
		{
			if (info->location + 1 >= text_record_start + MAX_TEXT_RECORD_LENGTH / 2)
			{
				finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
				text_record_start = info->location;
				text_record_address = 0;
			}

			char c[3];
			snprintf(c, 3, "%.2X", info->operation.opcode);
			memcpy(current_record(&program)->object_code + text_record_address, c, 2);
			text_record_address += 2;
			break;
		}

		case 2:
			if (info->operand.length == 3)
			{
				struct symbol_table_entry* r1 = map_getn(&state->symbol_table, info->operand.start, 1);
				struct symbol_table_entry* r2 = map_getn(&state->symbol_table, info->operand.start + 2, 1);
				if (r1 && r2)
				{
					if (info->location + 2 >= text_record_start + MAX_TEXT_RECORD_LENGTH / 2)
					{
						finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
						text_record_start = info->location;
						text_record_address = 0;
					}

					char c[5];
					snprintf(c, 5, "%.4X",
						(info->operation.opcode << 8) | (r1->value << 4) | r2->value
					);
					memcpy(current_record(&program)->object_code + text_record_address, c, 4);
					text_record_address += 4;
					break;
				}
			}
			else if (info->operand.length == 1)
			{
				struct symbol_table_entry* r1 = map_getn(&state->symbol_table, info->operand.start, 1);
				if (r1)
				{
					if (info->location + 2 >= text_record_start + MAX_TEXT_RECORD_LENGTH / 2)
					{
						finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
						text_record_start = info->location;
						text_record_address = 0;
					}

					char c[5];
					snprintf(c, 5, "%.4X",
						(info->operation.opcode << 8) | (r1->value << 4)
					);
					memcpy(current_record(&program)->object_code + text_record_address, c, 4);
					text_record_address += 4;
					break;
				}
			}
			info->error = "invalid register format";
			break;

		case 3:
		{
			unsigned int code = (info->operation.opcode << 16) & 0b111111000000000000000000;

			if (info->operand.start)
			{
				long target = parse_operand_2(state, info->operand);
				code |= calculate_target_address(info, target);
			}
			else
			{
				code |= (0b110000 << 12);
			}

			if (info->error)
			{
				continue;
			}

			if (info->location + 3 >= text_record_start + MAX_TEXT_RECORD_LENGTH / 2)
			{
				finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
				text_record_start = info->location;
				text_record_address = 0;
			}

			char c[7];
			snprintf(c, 7, "%.6X", code);
			memcpy(current_record(&program)->object_code + text_record_address, c, 6);
			text_record_address += 6;
			break;
		}

		case 4:
		{
			unsigned int code = (info->operation.opcode << 24) & 0b11111100000000000000000000000000;

			if (info->operand.start)
			{
				long target = parse_operand_2(state, info->operand);
				code |= calculate_target_address(info, target);
			}

			if (info->location + 4 >= text_record_start + MAX_TEXT_RECORD_LENGTH / 2)
			{
				finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
				text_record_start = info->location;
				text_record_address = 0;
			}

			char c[9];
			snprintf(c, 9, "%.8X", code);
			memcpy(current_record(&program)->object_code + text_record_address, c, 8);
			text_record_address += 8;
			break;
		}

		case 5:
		{
			const unsigned int len = info->operation.opcode;

			if (len >= MAX_TEXT_RECORD_LENGTH / 2)
			{
				info->error = "constant too long";
				continue;
			}
			
			if (text_record_address + 2 * len >= MAX_TEXT_RECORD_LENGTH)
			{
				finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
				text_record_start = info->location;
				text_record_address = 0;
			}

			char c[61];
			snprintf(c, 2 * len + 1, "%.*X", 2 * len, info->operand.start ? parse_operand_2(state, info->operand) : info->location);
			memcpy(current_record(&program)->object_code + text_record_address, c, 2 * len);
			text_record_address += 2 * len;
			break;
		}

		case 6:
		{
			if (text_record_address + 6 >= MAX_TEXT_RECORD_LENGTH)
			{
				finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
				text_record_start = info->location;
				text_record_address = 0;
			}

			char c[7];
			snprintf(c, 7, "%.6X", parse_operand_2(state, info->operand));
			memcpy(current_record(&program)->object_code + text_record_address, c, 6);
			text_record_address += 6;
			break;
		}
		}
	}

	if (text_record_address > 0)
	{
		finish_text_record(&program, text_record_start, text_record_start + text_record_address / 2);
	}
	array_remove(&program.text_records, array_size(&program.text_records) - 1);

	bool err = false;
	for (int i = 0; i < array_size(&state->line_infos); i++)
	{
		struct line_info* info = array_at(&state->line_infos, i);
		if (info->error)
		{
			fprintf(stderr, "[line %d]: %.*s\n%s\n\n", i, info->line.length - 1, info->line.start, info->error);

			err = true;
		}
	}

	char* c = NULL;

	if (!err)
	{
		const size_t text_size = sizeof(struct text_record) * array_size(&program.text_records);
		c = malloc(sizeof(struct header_record) + text_size + sizeof(struct end_record) + 1);

		memcpy(c, &program.header_record, sizeof(program.header_record));
		memcpy(c + sizeof(program.header_record), array_at(&program.text_records, 0), text_size);
		memcpy(c + sizeof(program.header_record) + text_size, &program.end_record, sizeof(program.end_record));
		c[sizeof(program.header_record) + text_size + sizeof(program.end_record)] = '\0';
	}

	array_del(&program.text_records);
	array_del(&lines);

	return c;
}

void parse_line(struct assembler_state* state, struct str line, unsigned int linenum)
{
	struct array tokens = split_line(line);

	struct line_info info;
	memset(&info, 0, sizeof(struct line_info));
	info.flags[FLAG_N] = true;
	info.flags[FLAG_I] = true;
	info.line_number = linenum;
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
	else if (sz == 1) {
		parse_operation(state, *(struct str*)array_at(&tokens, 0), linenum);
	}
	else if (sz > 0)
		((struct line_info*)array_at(&state->line_infos, array_size(&state->line_infos) - 1))->error = "invalid syntax";

	array_del(&tokens);
}

void parse_label(struct assembler_state* state, struct str label, unsigned int linenum)
{
	((struct line_info*)array_at(&state->line_infos, array_size(&state->line_infos) - 1))->label = label;

	if (map_getn(&state->symbol_table, label.start, label.length))
		((struct line_info*)array_at(&state->line_infos, array_size(&state->line_infos) - 1))->error = "multiple defitions for label";
	else {
		struct symbol_table_entry entry = { .line_number = linenum, .value = *assembler_state_location_counter(state) };
		map_setn(&state->symbol_table, label.start, label.length, &entry);
	}
}

void parse_operation(struct assembler_state* state, struct str operation, unsigned int linenum)
{
	struct operation_table_entry* op = map_getn(&state->operation_table, operation.start, operation.length);

	if (op)
		((struct line_info*)array_at(&state->line_infos, array_size(&state->line_infos) - 1))->operation = *op;
	else
		((struct line_info*)array_at(&state->line_infos, array_size(&state->line_infos) - 1))->error = "unknown operation";
}

void parse_operand(struct assembler_state* state, struct str operand, unsigned int linenum)
{
	struct line_info* info = array_at(&state->line_infos, array_size(&state->line_infos) - 1);

	switch (info->operation.format) {
	case 4:
		info->flags[FLAG_E] = true;
		// fall through here
	case 0:
	case 3:
		if (operand.length > 2 && operand.start[operand.length - 2] == ',' && operand.start[operand.length - 1] == 'X') {
			info->flags[FLAG_N] = true;
			info->flags[FLAG_I] = true;
			info->flags[FLAG_X] = true;

			// trim ,X off the end
			operand.length -= 2;
		}
		else if (operand.start[0] == '#') {
			info->flags[FLAG_N] = false;
			info->flags[FLAG_I] = true;
			info->flags[FLAG_X] = false;

			// trim # off the beginning
			operand.start += 1;
			operand.length -= 1;
		}
		else if (operand.start[0] == '@') {
			info->flags[FLAG_N] = true;
			info->flags[FLAG_I] = false;
			info->flags[FLAG_X] = false;

			// trim @ off the beginning
			operand.start += 1;
			operand.length -= 1;
		}
		else if (operand.start[0] == '=') {
			info->flags[FLAG_N] = true;
			info->flags[FLAG_I] = true;
			info->flags[FLAG_X] = false;

			const unsigned int value = parse_operand_2(
				state,
				(struct str) {
				.start = operand.start + 1, .length = operand.length - 1
			}
			);

			struct literal_table_entry literal =
			{
				.value = value,
				.address = -1
			};

			bool found = false;
			for (size_t i = 0; i < array_size(&state->literals); i++)
			{
				struct literal_table_entry* entry = array_at(&state->literals, i);
				if (value == entry->value)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				array_append(&state->literals, &literal);
			}
		}
		break;
	}

	// calculate these later during target address calculation
	info->flags[FLAG_P] = false;
	info->flags[FLAG_B] = false;

	info->operand = operand;
}

unsigned int parse_operand_2(struct assembler_state* state, struct str s)
{
	if (s.start[0] == '=')
	{
		const unsigned int value = parse_operand_2(
			state,
			(struct str){ .start = s.start + 1, .length = s.length - 1 }
		);

		for (size_t i = 0; i < array_size(&state->literals); i++)
		{
			struct literal_table_entry* literal = array_at(&state->literals, i);
			if (value == literal->value)
			{
				return literal->address;
			}
		}

		return 0;
	}
	else if (s.length > 3 && (s.start[0] == 'C' || s.start[0] == 'X') && s.start[1] == '\'' && s.start[s.length - 1] == '\'')
	{
		unsigned int value = 0;

		if (s.start[0] == 'C')
		{
			for (int offset = 2; offset < s.length - 1; offset++)
			{
				value = (value << 8) + s.start[offset];
			}
		}
		else
		{
			for (int offset = 2; offset < s.length - 1; offset++)
			{
				unsigned int cval;
				if (s.start[offset] >= '0' && s.start[offset] <= '9')
				{
					cval = s.start[offset] - '0';
				}
				else if (s.start[offset] >= 'A' && s.start[offset] <= 'F')
				{
					cval = s.start[offset] - 'A' + 10;
				}
				else if (s.start[offset] >= 'a' && s.start[offset] <= 'f')
				{
					cval = s.start[offset] - 'a' + 10;
				}
				else
				{
					continue;
				}

				value = (value << 4) + cval;
			}
		}

		return value;
	}

	struct symbol_table_entry* entry = map_getn(&state->symbol_table, s.start, s.length);
	if (entry)
	{
		return entry->value;
	}
	else
	{
		bool all_numeric = true;
		for (int i = 0; i < s.length; i++)
		{
			if (s.start[i] < '0' || s.start[i] > '9')
			{
				all_numeric = false;
				break;
			}
		}

		if (all_numeric)
		{
			return decimal_to_decimal(s);
		}
		else
		{
			// split string by -
			struct array tokens = split_operand(s);

			unsigned int value = 0;
			if (array_size(&tokens) >= 3 && array_size(&tokens) % 2 == 1)
			{
				value = parse_operand_2(state, *(struct str*)array_at(&tokens, 0));
				for (int current = 1; current + 1 < array_size(&tokens); current += 2)
				{
					struct str operation = *(struct str*)array_at(&tokens, current);
					struct str operand = *(struct str*)array_at(&tokens, current + 1);

					switch (operation.start[0])
					{
					case '+':
						value += parse_operand_2(state, operand);

					case '-':
						value -= parse_operand_2(state, operand);
						break;

					default:
						array_del(&tokens);
						return 0;
					}
				}
			}

			array_del(&tokens);

			return value;
		}
	}
}

void finish_text_record(struct program* p, unsigned int start, unsigned int end)
{
	char buf[7] = { '0', '0', '0', '0', '0', '0', '0' };
	snprintf(buf, 7, "%.6X", start);
	memcpy(current_record(p)->object_code_start, buf, 6);
	snprintf(buf, 3, "%.2X", end - start);
	memcpy(current_record(p)->object_code_length, buf, 2);

	struct text_record text;
	memset(&text, '0', sizeof(text));
	memset(&text.object_code, ' ', sizeof(text.object_code));
	text.magic_number = 'T';
	text.newline = '\n';
	array_append(&p->text_records, &text);
}

struct text_record* current_record(struct program* p)
{
	return array_at(&p->text_records, array_size(&p->text_records) - 1);
}

unsigned int calculate_target_address(struct line_info* info, long target)
{
	unsigned int code = 0;

	if (info->flags[FLAG_E])
	{
		code |= (0b000001 << 20);
	}
	else
	{
		if (target >= 0 && target <= 4095 && !info->flags[FLAG_N] && info->flags[FLAG_I])
		{
			// immediate mode is neither pc nor base relative by default
		}
		else if (target - (long)info->location >= -2047 && target - (long)info->location <= 2048)
		{
			// pc relative
			info->flags[FLAG_P] = true;
			target -= info->location + info->operation.format;
			code |= (0b000010 << 12);
		}
		else if (info->base >= 0 && target - (long)info->base >= 0 && target - (long)info->base <= 4095)
		{
			// base relative
			info->flags[FLAG_B] = true;
			target -= info->base;
			code |= (0b00100 << 12);
		}
		else if (target >= 0 && target <= 4095)
		{
			// neither
		}
		else
		{
			info->error = "can't reach target address";
		}
	}

	if (info->flags[FLAG_N] && info->flags[FLAG_I] && info->flags[FLAG_X])
	{
		code |= (0b001000 << (info->flags[FLAG_E] ? 20 : 12));
	}
	
	if (info->flags[FLAG_N])
	{
		code |= (0b100000 << (info->flags[FLAG_E] ? 20 : 12));
	}
	
	if (info->flags[FLAG_I])
	{
		code |= (0b010000 << (info->flags[FLAG_E] ? 20 : 12));
	}

	code |= (target & (info->flags[FLAG_E] ? 0b11111111111111111111 : 0b111111111111));

	return code;
}

void place_literals(struct assembler_state* state)
{
	for (size_t i = 0; i < array_size(&state->literals); i++)
	{
		struct literal_table_entry* literal = array_at(&state->literals, i);
		if (literal->address == -1)
		{
			literal->address = *assembler_state_location_counter(state);

			unsigned int tmp = literal->value;
			unsigned int len = 0;
			while (tmp > 0)
			{
				tmp /= 2;
				len += 1;
			}

			struct line_info info;
			memset(&info, 0, sizeof(struct line_info));
			info.operation.format = 5;
			info.operation.opcode = (len + 7) / 8;
			info.location = literal->value;
			array_append(&state->line_infos, &info);

			*assembler_state_location_counter(state) += info.operation.opcode;
		}
	}
}