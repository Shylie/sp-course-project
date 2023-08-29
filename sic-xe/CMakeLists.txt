cmake_minimum_required(VERSION 3.26)

project(sic-xe C)

add_executable(sic-xe
	source/main.c
)

set_target_properties(sic
	PROPERTIES
		C_STANDARD 90
)