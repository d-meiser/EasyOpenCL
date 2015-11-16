include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}
    )
include_directories(SYSTEM
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )

file(COPY
    runInteractive.sh
    sample.txt
    sampleBadPlatform.txt
    sampleBadDevice.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(EXAMPLES interactive stream)

foreach(e ${EXAMPLES})
  add_executable(${e} ${e}.c)
  target_link_libraries(${e} ecl_lib ${OpenCL_LIBRARIES})
endforeach()

if(ECL_RUN_EXAMPLES_AS_TEST)
  add_test(interactive sh runInteractive.sh)
  add_test(stream stream)
endif()
