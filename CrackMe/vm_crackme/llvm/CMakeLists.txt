cmake_minimum_required(VERSION 3.14)
project(llvm_devirt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_PREFIX_PATH
    "${CMAKE_SOURCE_DIR}/llvm-project/build/lib/cmake/llvm"
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include: ${LLVM_INCLUDE_DIRS}")

string(REPLACE "-fno-exceptions" "" LLVM_DEFINITIONS "${LLVM_DEFINITIONS}")
string(REPLACE "-fno-rtti"       "" LLVM_DEFINITIONS "${LLVM_DEFINITIONS}")

llvm_map_components_to_libnames(llvm_libs
    core
    support
    irreader
    passes
    analysis
    transformutils
    bitwriter
    bitreader
)

add_library(devirt_lib
    src/disassembler.cpp
    src/lifter.cpp
    src/vm_handlers.cpp
    src/devirtualizer.cpp
)

target_include_directories(devirt_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(devirt_lib PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(devirt_lib PRIVATE ${llvm_libs})

add_executable(llvm_devirt main.cpp)
target_link_libraries(llvm_devirt PRIVATE devirt_lib)