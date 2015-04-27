include_directories(${CMAKE_CURRENT_SOURCE_DIR}) #Include source header files

add_subdirectory(visitor)
add_subdirectory(export)
add_subdirectory(core)

set(SRCS
    demo.cpp
    main.cpp

    demo.h
    )

add_executable(CppToolMain ${SRCS})
target_link_libraries(CppToolMain core export clangTooling)
