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

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cpptool-proto/CMakeLists.txt)
    message(FATAL_ERROR "Please initialize the cpptool-proto submodule: 'git submodule update --init'")
endif()

if( MSVC )
    # Disable various compilation warnings caused by clang in MSVC
    # while it is possible to disable these locally through #pragma's
    # the include locations are too wide-spread through the code-base
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
    add_definitions(/wd4800 /wd4291 /wd4244 /wd4146)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
endif( MSVC )

# Include Protobuf subproject
add_subdirectory(cpptool-proto)
include_directories(${CPPTOOL_PROTO_INCLUDES})

add_subdirectory(src)
