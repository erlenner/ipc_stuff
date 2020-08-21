# CMakeLists.txt
cmake_minimum_required (VERSION 3.5.1)
project (icp_demo)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

set(EXAMPLE "examples/ring_queue_demo")
include_directories(.)

add_executable(icpprod ${EXAMPLE}/prod.c)
target_link_libraries(icpprod -lrt)
install(TARGETS icpprod DESTINATION bin)

add_executable(icpcons ${EXAMPLE}/cons.c)
target_link_libraries(icpcons -lrt)
install(TARGETS icpcons DESTINATION bin)
