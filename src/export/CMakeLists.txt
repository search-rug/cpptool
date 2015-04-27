set(SRCS
    export_api.cpp
    export_api.hpp

    dump_export.cpp
    dump_export.hpp

    export_util.cpp
    export_util.hpp
    )

add_library(export STATIC ${SRCS})
target_link_libraries(export LLVMSupport)