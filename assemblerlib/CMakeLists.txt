cmake_minimum_required(VERSION 3.18)

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

target_link_libraries(assemblerlib
	PUBLIC
		commonlib
)