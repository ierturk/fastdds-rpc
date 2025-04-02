cmake_minimum_required(VERSION 3.15)
project(rbic1_rpc LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom command for fastddsgen
set(FASTDDSGEN_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/resource/idl/RBIC1Rpc.idl)
set(FASTDDSGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/fastdds-gen)

add_custom_command(
    OUTPUT ${FASTDDSGEN_OUTPUT_DIR}/RBIC1Rpc.cxx ${FASTDDSGEN_OUTPUT_DIR}/RBIC1RpcPubSubTypes.cxx
    COMMAND fastddsgen -d ${FASTDDSGEN_OUTPUT_DIR} ${FASTDDSGEN_IDL_FILE} -cs
    DEPENDS ${FASTDDSGEN_IDL_FILE}
    COMMENT "Generating Fast DDS code from ${FASTDDSGEN_IDL_FILE}"
    VERBATIM
)

# Add generated files to the sources
set(GENERATED_SOURCES
    ${FASTDDSGEN_OUTPUT_DIR}/RBIC1Rpc.cxx
    ${FASTDDSGEN_OUTPUT_DIR}/RBIC1RpcPubSubTypes.cxx
)

# Detect architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Building for 64-bit architecture")
    set(IS_64BIT TRUE)
else()
    message(STATUS "Building for 32-bit architecture")
    set(IS_64BIT FALSE)
endif()

# Define sources and headers based on architecture
if(IS_64BIT)
    # 64-bit build: Include only invoker functionality
    set(SOURCES
        src/RBIC1Invoker.cpp
        ${GENERATED_SOURCES}
    )
    set(HEADERS
        include/RBIC1Invoker.h
    )
else()
    # 32-bit build: Include only proxy functionality
    set(SOURCES
        src/RBIC1Proxy.cpp
        ${GENERATED_SOURCES}
    )
    set(HEADERS
        include/RBIC1Proxy.h
    )
endif()

# Create the library
add_library(rbic1_rpc STATIC ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(rbic1_rpc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/rpcdds
    ${FASTDDSGEN_OUTPUT_DIR}
)

# Link dependencies
find_package(fastcdr REQUIRED)
find_package(fastdds REQUIRED)

target_link_libraries(rbic1_rpc PUBLIC
    fastcdr
    fast-dds::fast-dds
)

# Link RBIC1.lib and copy RBIC1.dll for 32-bit builds
if(NOT IS_64BIT)
    target_include_directories(rbic1_rpc PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/lib
    )

    target_link_libraries(rbic1_rpc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/resource/lib/RBIC1.lib)

    add_custom_command(TARGET rbic1_rpc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resource/lib/RBIC1.dll
        $<TARGET_FILE_DIR:rbic1_rpc>
    )
endif()

# Add compile definitions if needed
target_compile_definitions(rbic1_rpc PUBLIC
    $<BUILD_INTERFACE:FASTRTPS_NO_LIB>
)
