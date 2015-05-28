include_directories(${CMAKE_CURRENT_SOURCE_DIR}) #Include source header files

add_subdirectory(visitor)
add_subdirectory(export)
add_subdirectory(core)

set(SRCS
    main.cpp
    )

add_executable(CppToolMain ${SRCS})
target_link_libraries(CppToolMain core export clangTooling)
