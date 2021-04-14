cmake_minimum_required(VERSION 3.10)

project(Tetris)

file(GLOB SRC_FILES
	src/*
)

file(GLOB SFML_RELEASE_LIBS
	libs/SFML/lib/Release/*.lib
)

file(GLOB SFML_DEBUG_LIBS
	libs/SFML/lib/Debug/*.lib
)

add_compile_definitions(SFML_STATIC)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "libs/SFML/include")

target_link_libraries(${PROJECT_NAME} 
	optimized "${SFML_RELEASE_LIBS};opengl32.lib;winmm.lib;legacy_stdio_definitions.lib;"
	debug "${SFML_DEBUG_LIBS};opengl32.lib;winmm.lib;legacy_stdio_definitions.lib;")
