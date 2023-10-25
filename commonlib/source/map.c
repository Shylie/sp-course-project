#include "commonlib.h"

static char upper(char c)
{
	if (c >= 'a' && c <= 'z')
	{
		return c + ('A' - 'a');
	}
	else
	{
		return c;
	}
}

static int subhash(char c)
{
	const char uc = upper(c);
	if (uc >= 'A' && uc <= 'Z')
	{
		return uc - 'A';
	}
	else
	{
		return 26;
	}
}

static int hash(const char* str, size_t strlen)
{
	return subhash(str[0]) + 27 * subhash(str[strlen - 1]);
}

static char** get_key(void* elem, size_t elem_size)
{
	const size_t offset = elem_size;
	return (char*)(((uint8_t*)elem) + offset);
}

struct map map_new(size_t element_size)
{
	struct map m = { .element_size = element_size };

	for (int i = 0; i < 27 * 27; i++)
	{
		m.bins[i] = array_new(element_size + sizeof(char*), 0);
	}
	
	return m;
}

static void del_key(void* elem, size_t elem_size, void* ud)
{
	char** key = get_key(elem, elem_size - sizeof(char*));
	cl_realloc(key, 0);
}

void map_del(struct map* m)
{

	for (int i = 0; i < 27 * 27; i++)
	{
		array_foreach(&m->bins[i], del_key, NULL);
		array_del(&m->bins[i]);
	}
}

struct find_info
{
	const char* key;
	size_t keylen;
	void* elem;
};

static void compare_elem(void* elem, size_t elem_size, struct find_info* ud)
{
	if (!ud->elem)
	{
		const char** key = get_key(elem, elem_size - sizeof(char*));
		if (strlen(*key) == ud->keylen && strncmp(ud->key, *key, ud->keylen) == 0)
		{
			ud->elem = elem;
		}
	}
}

void* map_get(struct map* m, const char* key, size_t keylen)
{
	struct find_info elem = { .key = key, .keylen = keylen, .elem = NULL };
	array_foreach(&m->bins[hash(key, keylen)], compare_elem, &elem);

	return elem.elem;
}

void map_set(struct map* m, const char* key, size_t keylen, const void* elem)
{
	void* found = map_get(m, key, keylen);

	if (!found)
	{
		// not found, allocate
		const size_t bin = hash(key, keylen);
		const size_t index = array_size(&m->bins[bin]);
		array_resize(&m->bins[bin], index + 1);
		found = array_at(&m->bins[bin], index);
		char** key2 = get_key(found, m->element_size);
		*key2 = NULL;
		cl_realloc(key2, keylen + 1);
		strncpy(*key2, key, keylen);
		(*key2)[keylen] = '\0';
		m->bins[bin].used_data_size += m->bins[bin].element_size;
	}

	// only use element_size to avoid overwiting key
	memmove(found, elem, m->element_size);
}