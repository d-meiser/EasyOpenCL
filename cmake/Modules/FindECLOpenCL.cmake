# Try to find OpenCL library and headers.
#
# Use OpenCL_ROOT_DIR to specify location of cgreen
# When successful this defines
# OpenCL_FOUND
# OpenCL_LIB
# OpenCL_INCLUDE_DIR

find_path(OpenCL_INCLUDE_DIR CL/cl.h
    HINTS ${OpenCL_ROOT_DIR}/include;/usr/include;/usr/local/include)
find_library(OpenCL_LIB OpenCL
    HINTS ${OpenCL_ROOT_DIR}/lib64;/usr/local/lib;/usr/lib;/usr/lib64)
if (OpenCL_INCLUDE_DIR AND OpenCL_LIB)
  set(OpenCL_FOUND TRUE)
  set(ECLOpenCL_FOUND TRUE)
endif ()
message(STATUS "OpenCL_FOUND == ${OpenCL_FOUND}")
message(STATUS "OpenCL_LIB == ${OpenCL_LIB}")
message(STATUS "OpenCL_INCLUDE_DIR == ${OpenCL_INCLUDE_DIR}")
