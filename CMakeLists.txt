cmake_minimum_required(VERSION 3.10)

project(Tetris)

set(SRC_FILES
	src/main.cpp
	src/Tetris.hpp
	src/Tetris.cpp
)

add_subdirectory(external/SFML)

if(NOT TARGET sfml-graphics)
	message(FATAL_ERROR "sfml-graphics is required!")
elseif(NOT TARGET sfml-system)
	message(FATAL_ERROR "sfml-system is required!")
elseif(NOT TARGET sfml-window)
	message(FATAL_ERROR "sfml-window is required!")
endif()

if(NOT BUILD_SHARED_LIBS)
	add_compile_definitions(SFML_STATIC)
endif()

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE "external/SFML/include")

target_link_libraries(${PROJECT_NAME} "sfml-graphics;sfml-system;sfml-window")

set_target_properties( ${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)