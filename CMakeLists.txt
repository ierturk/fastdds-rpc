cmake_minimum_required(VERSION 3.15)
project(idl-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fastdds REQUIRED)
find_package(fastcdr REQUIRED)

# Add subdirectory for rbic1_rpc
add_subdirectory(rbic1_rpc)

# Source files
set(SOURCES
    src/main.cpp
)

# Create the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories for rbic1_rpc
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/rbic1_rpc/include
)

# Link the rbic1_rpc library
target_link_libraries(${PROJECT_NAME} PRIVATE rbic1_rpc)

# Copy the RBIC1.dll file to the output directory after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/rbic1_rpc/resource/lib/RBIC1.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
