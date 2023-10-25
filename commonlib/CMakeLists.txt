cmake_minimum_required(VERSION 3.18)

project(commonlib C)

add_library(commonlib STATIC
	include/commonlib.h

	source/commonlib.c
	source/array.c
	source/map.c
)

set_target_properties(commonlib
	PROPERTIES
		C_STANDARD 99
)

target_include_directories(commonlib
	PUBLIC
		include
)