cmake_minimum_required(VERSION 2.6)

set(SOURCES
	main.cpp
	src/impl.cpp
	src/GenerationKeyPair.cpp
	src/Signing.cpp
	src/Verification.cpp
	src/rsa_helper.cpp
	src/ByteStorage.cpp
)

set(CMAKE_CXX_STANDARD 14)

add_executable("ds" ${SOURCES})
