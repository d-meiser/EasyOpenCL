include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}
    )
include_directories(SYSTEM 
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )

file(COPY runInteractive.sh sample.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(interactive interactive.c)
target_link_libraries(interactive ecl_lib ${OpenCL_LIBRARIES})
if(ECL_RUN_EXAMPLES_AS_TEST)
  add_test(interactive sh runInteractive.sh)
endif()
