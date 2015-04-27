set(SRCS
    runtime_context.cpp
    runtime_context.hpp

    clang_interface_action.cpp
    clang_interface_consumer.cpp
    clang_interface.hpp
    )

add_library(core STATIC ${SRCS})
target_link_libraries(core visitor clangTooling)