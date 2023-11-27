#ifndef COMMONLIB_H
#define COMMONLIB_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void cl_realloc(void** ptr, size_t size);

struct array
{
	size_t element_size;
	uint8_t *data;
	size_t data_size;
	size_t used_data_size;
};

struct array array_new(size_t element_size, size_t initial_count);
void   array_del(struct array* arr);
size_t array_size(const struct array* arr);
size_t array_max_size(const struct array* arr);
void   array_resize(struct array* arr, size_t new_max_size);
void   *array_at(struct array* arr, size_t index);
void   *array_find(struct array* arr, const void* element_to_find);
void   array_append(struct array* arr, const void* element);
void   array_remove(struct array* arr, size_t index);
void   array_foreach(struct array* arr, void (*fn)(void* elem, size_t elem_size, void* ud), void* ud);

struct map
{
	struct array bins[27 * 27];
	size_t element_size;
};

struct map map_new(size_t element_size);
void map_del(struct map* m);
void* map_get(struct map* m, const char* key);
void *map_getn(struct map* m, const char* key, size_t keylen);
void map_set(struct map* m, const char* key, const void* elem);
void map_setn(struct map* m, const char* key, size_t keylen, const void* elem);
void map_foreach(struct map* m, void (*fn)(const char* key, void* elem, void* ud), void* ud);

#endif//COMMONLIB_H
