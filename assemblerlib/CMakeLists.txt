cmake_minimum_required(VERSION 3.26)

project(assemblerlib C)

add_library(assemblerlib STATIC
	include/assemblerlib.h

	source/assemblerlib-private.h
	source/assemblerlib.c
	source/constants.c
)

set_target_properties(assemblerlib
	PROPERTIES
		C_STANDARD 99
)

target_include_directories(assemblerlib
	PUBLIC
		include
)