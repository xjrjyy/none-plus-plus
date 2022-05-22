cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(NonePlusPlus VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
# set(ENABLE_EXPORTS on)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)

enable_testing()

find_package(LLVM REQUIRED HINTS "${LLVM_CMAKE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

include(AddLLVM)
include(HandleLLVMOptions)

include_directories("${LLVM_BINARY_DIR}/include" "${LLVM_INCLUDE_DIR}")
link_directories("${LLVM_LIBRARY_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
include(AddNpp)

add_subdirectory(third_party/fmt EXCLUDE_FROM_ALL)
add_subdirectory(third_party/cxxopts EXCLUDE_FROM_ALL)
add_subdirectory(third_party/catch2 EXCLUDE_FROM_ALL)
add_subdirectory(src)
add_subdirectory(tests)
message("${CMAKE_INSTALL_FULL_BINDIR}")
