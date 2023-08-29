cmake_minimum_required(VERSION 3.26)

project(sic C)

add_executable(sic
	source/main.c
)

set_target_properties(sic
	PROPERTIES
		C_STANDARD 90
)