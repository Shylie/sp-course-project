cmake_minimum_required(VERSION 3.26)

project(interpreter C)

add_executable(interpreter
	source/interpreter.c
)

set_target_properties(interpreter
	PROPERTIES
		C_STANDARD 99
)

target_link_libraries(interpreter
	PRIVATE
		assemblerlib
)