cmake_minimum_required(VERSION 3.15)
project(fastdds-qml-app LANGUAGES CXX)

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

    # Qt configuration
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    # Find dependencies (installed by Conan)
    find_package(Qt6 COMPONENTS Core Quick Qml REQUIRED)
    find_package(Boost REQUIRED COMPONENTS system)

    set(INVOKER_SOURCES
        src/invoker_main.cpp
        src/RBIC1RpcBridge.cpp
        src/RBIC1RpcBridge.h
    )

    add_executable(invoker_app
        ${INVOKER_SOURCES}
    )

    qt_add_qml_module(invoker_app
        URI TaskHandlerModule
        VERSION 1.0
        QML_FILES
            src/qml/main.qml
    )

    # Include directories for rbic1_rpc
    target_include_directories(invoker_app PRIVATE
        src
        ${CMAKE_SOURCE_DIR}/rbic1_rpc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/mpscchannel
    )

    # Link the rbic1_rpc library
    target_link_libraries(invoker_app PRIVATE
        rbic1_rpc
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Boost::system
    )

    # Windows-specific deployment
    if(WIN32)
        # Copy Qt plugins for deployment
        get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
        get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

        add_custom_command(TARGET invoker_app POST_BUILD
            COMMAND "${_qt_bin_dir}/windeployqt.exe"
                    --qmldir "${CMAKE_CURRENT_SOURCE_DIR}/src/qml"
                    "$<TARGET_FILE:invoker_app>"
            COMMENT "Deploying Qt dependencies..."
        )
    endif()

endif()
