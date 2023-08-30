cmake_minimum_required(VERSION 3.26)

project(assembler C)

add_executable(assembler
	include/assembler.h

	source/assembler.c
)

set_target_properties(assembler
	PROPERTIES
		C_STANDARD 99
)

target_include_directories(assembler
	PUBLIC
		include
)