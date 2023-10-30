#include "assemblerlib.h"
#include "assemblerlib-private.h"

void al_init(void)
{
	OPERATION_TABLE = map_new(sizeof(struct operation_table_entry));
	SYMBOL_TABLE = map_new(sizeof(struct symbol_table_entry));

	al_fill_tables();
}

void al_deinit(void)
{
	map_del(&OPERATION_TABLE);
	map_del(&SYMBOL_TABLE);
}