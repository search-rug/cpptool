option(CPPTOOL_PRINT_PROTOBUF "Dump protobuf printout to STDOUT during exporting." OFF)

set(SRCS
    export_api.cpp
    export_api.hpp

    export_util.cpp
    export_util.hpp

    protobuf_output.cpp
    protobuf_output.hpp
    
    protobuf_export.cpp
    protobuf_export.hpp
    
    type_mapper.cpp
    type_mapper.hpp
    type_mapper.template.hpp
    )

if( CPPTOOL_PRINT_PROTOBUF )
    add_definitions(-DCPPTOOL_ENABLE_PROTOBUF_DUMP)
endif( CPPTOOL_PRINT_PROTOBUF )
    
add_library(export STATIC ${SRCS})
target_link_libraries(export ${CPPTOOL_PROTO_LIBRARY} LLVMSupport)