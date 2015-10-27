# Try to find OpenCL library and headers.
#
# Use OPENCL_ROOT_DIR to specify location of cgreen
# When successful this defines
# OPENCL_FOUND
# OPENCL_LIB
# OPENCL_INCLUDE_DIR

find_path(OPENCL_INCLUDE_DIR CL/cl.h
    HINTS ${OPENCL_ROOT_DIR}/include;/usr/include;/usr/local/include)
find_library(OPENCL_LIB OpenCL
    HINTS ${OPENCL_ROOT_DIR}/lib64;/usr/local/lib;/usr/lib;/usr/lib64)
if (OPENCL_INCLUDE_DIR AND OPENCL_LIB)
  set(OPENCL_FOUND TRUE)
endif ()
message(STATUS "OPENCL_FOUND == ${OPENCL_FOUND}")
message(STATUS "OPENCL_LIB == ${OPENCL_LIB}")
message(STATUS "OPENCL_INCLUDE_DIR == ${OPENCL_INCLUDE_DIR}")
