set(SRCS
    primary_visitor.cpp
    primary_visitor.hpp
    )

add_library(visitor STATIC ${SRCS})
target_link_libraries(visitor clangAST)