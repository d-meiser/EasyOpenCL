include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(SYSTEM 
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )

add_executable(testEasyOpenCL testEasyOpenCL.c)
target_link_libraries(testEasyOpenCL ${CGREEN_LIB})
add_test(testEasyOpenCL testEasyOpenCL)

