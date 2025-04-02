cmake_minimum_required(VERSION 3.15)
project(rbic1_rpc LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files for the library
set(SOURCES
    src/RBIC1Proxy.cpp
    src/RBIC1Invoker.cpp
)

# Add header files for the library
set(HEADERS
    include/RBIC1Proxy.h
    include/RBIC1Invoker.h
    # include/rpcdds.h
)

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
list(APPEND SOURCES
    ${FASTDDSGEN_OUTPUT_DIR}/RBIC1Rpc.cxx
    ${FASTDDSGEN_OUTPUT_DIR}/RBIC1RpcPubSubTypes.cxx
)

# Create the library
add_library(rbic1_rpc STATIC ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(rbic1_rpc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../common/rpcdds
    ${FASTDDSGEN_OUTPUT_DIR}
)

# Link the RBIC1.lib file
target_link_libraries(rbic1_rpc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/resource/lib/RBIC1.lib)

# Link dependencies (Fast DDS and other required libraries)
find_package(fastcdr REQUIRED)
find_package(fastdds REQUIRED)

target_link_libraries(rbic1_rpc PUBLIC
    fastcdr
    fastrtps
)

# Add compile definitions if needed
target_compile_definitions(rbic1_rpc PUBLIC
    $<BUILD_INTERFACE:FASTRTPS_NO_LIB>
)
