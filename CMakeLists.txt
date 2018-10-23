cmake_minimum_required(VERSION 2.6)

set(SOURCES
	main.cpp
	impl.cpp
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


add_executable("ds" ${SOURCES})
