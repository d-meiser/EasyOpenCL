add_executable(testEasyOpenCL testEasyOpenCL.c)
target_link_libraries(testEasyOpenCL ${CGREEN_LIB})
add_test(testEasyOpenCL testEasyOpenCL)

