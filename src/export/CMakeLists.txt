set(SRCS
    export_api.cpp
    export_api.hpp

    dump_export.cpp
    dump_export.hpp

    export_util.cpp
    export_util.hpp

    protobuf_output.cpp
    protobuf_output.hpp
    
    protobuf_export.cpp
    protobuf_export.hpp
    
    type_mapper.cpp
    type_mapper.hpp
    )

add_library(export STATIC ${SRCS})
target_link_libraries(export cppToolProto LLVMSupport)