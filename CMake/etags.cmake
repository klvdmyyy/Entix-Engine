macro(etags_generate)
  find_program(ETAGS_PATH etags)
  if(ETAGS_PATH)
    message(STATUS "etags found: ${ETAGS_PATH}")
    message(STATUS "Generating TAGS file")
    execute_process(
      COMMAND ${ETAGS_PATH} ${ARGV}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  else()
    message(STATUS "etags not found!")
  endif()
endmacro()