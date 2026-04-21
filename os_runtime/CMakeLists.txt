cmake_minimum_required(VERSION 3.16)
project(mini_os LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mini_os
    src/main.cpp
)

add_custom_target(iso
    COMMAND ${CMAKE_SOURCE_DIR}/tools/build_iso.sh ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/dist mini_os-runtime.iso
    DEPENDS mini_os
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Packaging mini_os-runtime.iso"
)

add_custom_target(bootable-iso
    COMMAND ${CMAKE_SOURCE_DIR}/bootable_iso/build_bootable_iso.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building bootable mini_os ISO via live-build"
)
