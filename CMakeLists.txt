cmake_minimum_required(VERSION 3.26)

project(sp-course-project)

add_executable(sp-course-project
	source/main.cpp
)

set_target_properties(sp-course-project
	PROPERTIES
		CXX_STANDARD 20
)