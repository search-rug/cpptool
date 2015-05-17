cmake_minimum_required(VERSION 3.1.3)
project(CppTool)

set(CMAKE_MODULE_PATH 
    ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake/"
    "${CMAKE_SOURCE_DIR}/cmake/Modules/"
    )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(CXX11 REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(CLANG REQUIRED CONFIG)
message(STATUS "Found Clang")
message(STATUS "Using ClangConfig.cmake in: ${CLANG_DIR}")

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

if( APPLE )
  add_definitions(-fno-rtti -fvisibility=hidden)
endif( APPLE )

# Include Protobuf subproject
add_subdirectory(CppTool-proto)
include_directories(${CPPTOOL_PROTO_INCLUDES})

add_subdirectory(src)
