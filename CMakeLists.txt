cmake_minimum_required(VERSION 2.6)

set(SOURCES
	main.cpp
	impl.cpp
	src/GenerationKeyPair.cpp
	src/Signing.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable("ds" ${SOURCES})
