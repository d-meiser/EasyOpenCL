include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${OpenCL_INCLUDE_DIR} ${CGREEN_INCLUDE_DIR})

add_library(ecl_lib ecl.c)

