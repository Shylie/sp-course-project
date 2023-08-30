#ifndef ASSEMBLERLIB_H
#define ASSEMBLERLIB_H

typedef struct
{
	unsigned char* buffer;
	unsigned int   buffer_size;
} al_machine_code;

al_machine_code assemble(const char* source);

#endif//ASSEMBLERLIB_H