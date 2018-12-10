cmake_minimum_required(VERSION 2.6)

SET(GCC_COVERAGE_LINK_FLAGS    "-pthread")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

set(SOURCES
	main.cpp
	src/impl.cpp
	src/GenerationKeyPair.cpp
	src/ByteStorage.cpp
	src/Signing.cpp
	src/Verification.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable("ds" ${SOURCES})
