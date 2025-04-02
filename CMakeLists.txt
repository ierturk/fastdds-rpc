cmake_minimum_required(VERSION 3.15)
project(idl-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectory for rbic1_rpc
add_subdirectory(rbic1_rpc)

# Detect architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for 64-bit architecture")
    set(IS_64BIT TRUE)
else()
    message(STATUS "Building for 32-bit architecture")
    set(IS_64BIT FALSE)
endif()

# Create the proxy app (only for 32-bit builds)
if(NOT IS_64BIT)
    set(PROXY_SOURCES
        src/proxy_main.cpp
    )

    add_executable(proxy_app ${PROXY_SOURCES})

    # Include directories for rbic1_rpc
    target_include_directories(proxy_app PRIVATE
        ${CMAKE_SOURCE_DIR}/rbic1_rpc/include
    )

    # Link the rbic1_rpc library
    target_link_libraries(proxy_app PRIVATE rbic1_rpc)

    # Copy the RBIC1.dll file to the output directory after build
    add_custom_command(TARGET proxy_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/rbic1_rpc/resource/lib/RBIC1.dll
        $<TARGET_FILE_DIR:proxy_app>
    )
endif()

# Create the invoker app (only for 64-bit builds)
if(IS_64BIT)
    set(INVOKER_SOURCES
        src/invoker_main.cpp
    )

    add_executable(invoker_app ${INVOKER_SOURCES})

    # Include directories for rbic1_rpc
    target_include_directories(invoker_app PRIVATE
        ${CMAKE_SOURCE_DIR}/rbic1_rpc/include
    )

    # Link the rbic1_rpc library
    target_link_libraries(invoker_app PRIVATE rbic1_rpc)
endif()
