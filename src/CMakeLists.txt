include_directories(${CMAKE_CURRENT_SOURCE_DIR}) #Include source header files

set(SRCS
    main.cpp
    )

add_executable(CppToolMain ${SRCS})
# do linking here!