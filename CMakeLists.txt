cmake_minimum_required(VERSION 3.15)
project(idl-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan setup
# include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

find_package(fastdds REQUIRED)
find_package(fastcdr REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp src/*.cxx src/*.h src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link Fast DDS
target_link_libraries(${PROJECT_NAME} PRIVATE fast-dds::fast-dds fastcdr fastrtps)

# Include the directory containing RBIC1.h
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/rbic1)

# Link the RBIC1.lib file
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/rbic1/RBIC1.lib)

# Copy the RBIC1.dll file to the output directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/rbic1/RBIC1.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)
