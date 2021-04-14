cmake_minimum_required(VERSION 3.10)

project(Tetris)

file(GLOB SRC_FILES
	src/*
)

add_subdirectory(external/SFML)

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "external/SFML/include")

add_dependencies(${PROJECT_NAME} SFML)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-main sfml-system sfml-window)