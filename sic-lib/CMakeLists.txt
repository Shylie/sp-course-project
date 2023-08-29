cmake_minimum_required(VERSION 3.26)

project(sic-lib C)

add_library(sic-lib STATIC
	include/sic-lib.h

	source/sic-lib.c
)

set_target_properties(sic-lib
	PROPERTIES
		C_STANDARD 90
)

target_include_directories(sic-lib
	PUBLIC
		include
)