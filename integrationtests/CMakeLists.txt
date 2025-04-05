# Enable testing
enable_testing()

# Qt configuration
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find dependencies (installed by Conan)
find_package(GTest REQUIRED)
find_package(Qt6 COMPONENTS Core Quick Qml REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Test sources
set(INTEGRATION_TEST_SOURCES
    test_main.cpp
    test_rbic1_rpc_bridge.cpp
    ../mocks/MockRBIC1RpcBridgeIfc.h
    ../src/RBIC1RpcBridgeIfc.h
)

# Create the test executable
add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})

# Include directories for tests
target_include_directories(integration_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/mocks
)

# Link the library and GTest/GMock to the test executable
target_link_libraries(integration_tests PRIVATE
    # rbic1_rpc
    gtest::gtest
    Boost::system
    Qt6::Core
    Qt6::Quick
)

# Add the test to CTest
add_test(NAME integration_tests COMMAND integration_tests)