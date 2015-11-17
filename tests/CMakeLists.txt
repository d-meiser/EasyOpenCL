include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}
    )
include_directories(SYSTEM 
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )

set(UNIT_TESTS
    GetSomeContext
    GetContextInteractively
    GetProgramFromSource)

foreach(t ${UNIT_TESTS})
  add_executable(test${t} test${t}.c)
  target_link_libraries(test${t} ecl_lib ${CGREEN_LIB} ${OpenCL_LIBRARIES})
  add_test(test${t} test${t})
endforeach()

file(COPY
    streamKernel.cl
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

