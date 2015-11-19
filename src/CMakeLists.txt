include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    )
include_directories(SYSTEM
    ${OpenCL_INCLUDE_DIR}
    ${CGREEN_INCLUDE_DIR}
    )

add_library(ecl_lib ecl.c)
set_target_properties(ecl_lib PROPERTIES DEFINE_SYMBOL BUILDING_ECL)

