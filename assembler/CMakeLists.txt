cmake_minimum_required(VERSION 3.18)

project(assembler C)

add_executable(assembler
	source/assembler.c
)

set_target_properties(assembler
	PROPERTIES
		C_STANDARD 99
)

target_link_libraries(assembler
	PRIVATE
		assemblerlib
)