include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}
    )
include_directories(SYSTEM 
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )
add_executable(interactive interactive.c)
if(ECL_RUN_EXAMPLES_AS_TEST)
  add_test(interactive interactive < 1 1)
endif()
