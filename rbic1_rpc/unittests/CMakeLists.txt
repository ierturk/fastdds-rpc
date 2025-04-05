# Enable testing
enable_testing()

# Find dependencies (installed by Conan)
find_package(GTest REQUIRED)

# Test sources
set(TEST_SOURCES
    test_main.cpp
    test_rpcdds.cpp
    test_rbic1_proxy.cpp
    test_rbic1_invoker.cpp
)

# Create the test executable
add_executable(rpcdds_unittests ${TEST_SOURCES})

# Include directories for tests
target_include_directories(rpcdds_unittests PRIVATE
    ${CMAKE_SOURCE_DIR}/rbic1_rpc/include
    ${CMAKE_SOURCE_DIR}/rbic1_rpc/mocks
)

# Link the library and GTest/GMock to the test executable
target_link_libraries(rpcdds_unittests PRIVATE
    rbic1_rpc
    gtest::gtest
)

# Add the test to CTest
add_test(NAME rpcdds_unittests COMMAND rpcdds_unittests)