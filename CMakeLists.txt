cmake_minimum_required(VERSION 2.6)

set(SOURCES
	main.cpp
	src/impl.cpp
	src/GenerationKeyPair.cpp
	src/Signing.cpp
	src/Verification.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable("ds" ${SOURCES})
