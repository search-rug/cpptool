function(GENERATE_EXTRA_PROTOBUF_LANGUAGE TARGET_NAME LANGUAGE_ARGS)
    add_custom_target(${TARGET_NAME})
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Additional Protobuf Languages") 
    
    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}")
    file(MAKE_DIRECTORY ${OUTPUT_DIR})

    # Find the include path that includes all the input files
    foreach(FIL ${ARGN})
      get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
      get_filename_component(ABS_PATH ${ABS_FIL} PATH)
      list(FIND _protobuf_include_path ${ABS_PATH} _contains_already)
      if(${_contains_already} EQUAL -1)
          list(APPEND _protobuf_include_path -I ${ABS_PATH})
      endif()
    endforeach()

    foreach(PROTO_FILE ${ARGN})
        get_filename_component(ABS_FILE ${PROTO_FILE} ABSOLUTE)
        
        add_custom_command(
          TARGET ${TARGET_NAME}
          COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
          ARGS ${LANGUAGE_ARGS} ${OUTPUT_DIR} ${_protobuf_include_path} ${ABS_FILE}
          DEPENDS ${ABS_FILE} ${PROTOBUF_PROTOC_EXECUTABLE}
          COMMENT "Running ${TARGET_NAME} protocol buffer compiler on ${PROTO_FILE}"
        )
    endforeach()
endfunction(GENERATE_EXTRA_PROTOBUF_LANGUAGE)