#include "commonlib.h"

void cl_realloc(void** ptr, size_t size)
{
	if (size > 0)
	{
		if (!*ptr)
		{
			*ptr = malloc(size);
		}
		else
		{
			void* ptr2 = realloc(*ptr, size);
			if (*ptr != ptr2)
			{
				*ptr = ptr2;
			}
		}
	}
	else
	{
		if (*ptr)
		{
			free(*ptr);
			*ptr = NULL;
		}
	}
}