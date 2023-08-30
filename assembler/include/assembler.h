#ifndef SIC_LIB_H
#define SIC_LIB_H

#include <stdlib.h>

struct Operation
{
	enum
	{
		// SIC instructions

		OP_ADD    = 0x18,
		OP_AND    = 0x40, 
		OP_COMP   = 0x28,
		OP_DIV    = 0x24,
		OP_J      = 0x3C,
		OP_JEQ    = 0x30,
		OP_JGT    = 0x34,
		OP_JLT    = 0x38,
		OP_JSUB   = 0x48,
		OP_LDA    = 0x00,
		OP_LDCH   = 0x50,
		OP_LDL    = 0x08,
		OP_LDX    = 0x04,
		OP_MUL    = 0x20,
		OP_OR     = 0x44,
		OP_RD     = 0xD8,
		OP_RSUB   = 0x4C,
		OP_STA    = 0x0C,
		OP_STCH   = 0x54,
		OP_STL    = 0x14,
		OP_STSW   = 0xE8,
		OP_STX    = 0x10,
		OP_SUB    = 0x1C,
		OP_TD     = 0xE0,
		OP_TIX    = 0x2C,
		OP_WD     = 0xDC,

		// SIC-XE instructions

		OP_ADDF   = 0x58,
		OP_ADDR   = 0x90,
		OP_CLEAR  = 0xB4,
		OP_COMPF  = 0x88,
		OP_COMPR  = 0xA0,
		OP_DIVF   = 0x64,
		OP_DIVR   = 0x9C,
		OP_FIX    = 0xC4,
		OP_FLOAT  = 0xC0,
		OP_HIO    = 0xF4,
		OP_LDB    = 0x68,
		OP_LDF    = 0x70,
		OP_LDS    = 0x6C,
		OP_LDT    = 0x74,
		OP_LPS    = 0xD0,
		OP_MULF   = 0x60,
		OP_MULR   = 0x98,
		OP_NORM   = 0xC8,
		OP_RMO    = 0xAC,
		OP_SHIFTL = 0xA4,
		OP_SHIFTR = 0xA8,
		OP_SIO    = 0xF0,
		OP_SSK    = 0xEC,
		OP_STB    = 0x78,
		OP_STF    = 0x80,
		OP_STI    = 0xD4,
		OP_STS    = 0x7C,
		OP_STT    = 0x84,
		OP_SUBF   = 0x5C,
		OP_SUBR   = 0x94,
		OP_SVC    = 0xB0,
		OP_TIO    = 0xF8,
		OP_TIXR   = 0xB8
	} code;
};

#endif//SIC_LIB_H