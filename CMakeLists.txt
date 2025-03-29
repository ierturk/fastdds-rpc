cmake_minimum_required(VERSION 3.15)
project(conan2-dds-test LANGUAGES CXX)

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
