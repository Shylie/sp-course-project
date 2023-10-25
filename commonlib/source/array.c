#include "commonlib.h"

struct array array_new(size_t element_size, size_t initial_count)
{
	struct array arr =
	{
		.element_size = element_size,
		.data = NULL,
		.data_size = initial_count * element_size,
		.used_data_size = 0
	};

	cl_realloc(&arr.data, arr.data_size);
	
	return arr;
}

void array_del(struct array* arr)
{
	cl_realloc(&arr->data, 0);
	arr->data_size = 0;
	arr->used_data_size = 0;
}

size_t array_size(const struct array* arr)
{
	return arr->used_data_size / arr->element_size;
}

size_t array_max_size(const struct array* arr)
{
	return arr->data_size / arr->element_size;
}

void array_resize(struct array* arr, size_t new_max_size)
{
	const size_t current_max_size = array_max_size(arr);
	if (new_max_size > current_max_size)
	{
		const size_t expected_new_size = current_max_size * 2;
		const size_t actual_new_max_size = (new_max_size > expected_new_size ? new_max_size : expected_new_size) * arr->element_size;
		cl_realloc(&arr->data, actual_new_max_size);
		arr->data_size = actual_new_max_size;
	}
}

void* array_at(struct array* arr, size_t index)
{
	return &arr->data[index * arr->element_size];
}

void array_append(struct array* arr, const void* element)
{
	const size_t sz = array_size(arr);
	array_resize(arr, sz + 1);

	memmove(array_at(arr, sz), element, arr->element_size);
	arr->used_data_size += arr->element_size;
}

void array_remove(struct array* arr, size_t index)
{
	const size_t elements_left = array_size(arr) - index - 1;
	if (elements_left > 0)
	{
		memmove(array_at(arr, index), array_at(arr, index + 1), elements_left * arr->element_size);
	}
	arr->used_data_size -= arr->element_size;
}

void* array_find(struct array* arr, const void* element_to_find)
{
	const size_t sz = array_size(arr);
	for (size_t i = 0; i < sz; i++)
	{
		void* current_elem = array_at(arr, i);
		if (memcmp(element_to_find, array_at(arr, i), arr->element_size) == 0)
		{
			return current_elem;
		}
	}

	return NULL;
}

void array_foreach(struct array* arr, void (*fn)(void* elem, size_t elem_size, void* ud), void* ud)
{
	const size_t sz = array_size(arr);
	for (size_t i = 0; i < sz; i++)
	{
		fn(array_at(arr, i), arr->element_size, ud);
	}
}