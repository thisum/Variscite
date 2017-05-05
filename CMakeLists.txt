cmake_minimum_required (VERSION 2.6)
project (cv_cam_test)
find_package( OpenCV REQUIRED )
add_executable (cv_cam_test cv_cam_test.cpp)
target_link_libraries( cv_cam_test ${OpenCV_LIBS} )
include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)

# Set CMAKE_C_FLAGS variable with info from pkg-util
execute_process(COMMAND pkg-config --cflags gstreamer-1.0
                OUTPUT_VARIABLE CMAKE_C_FLAGS)
string(REPLACE "\n" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")

# Set CMAKE_EXE_LINKER_FLAGS variable with info from pkg-util
execute_process(COMMAND pkg-config --libs gstreamer-1.0
                OUTPUT_VARIABLE CMAKE_EXE_LINKER_FLAGS)
string(REPLACE "\n" "" CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
message("CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

set_target_properties(cv_cam_test
                      PROPERTIES COMPILE_FLAGS ${CMAKE_C_FLAGS}
                                 LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
