# CMakeLists.txt
cmake_minimum_required (VERSION 3.5.1)
project (icp_demo)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

include_directories(include)



add_executable(ring_queue_prod examples/ring_queue_demo/prod.c)
target_link_libraries(ring_queue_prod -lrt)
install(TARGETS ring_queue_prod DESTINATION bin)

add_executable(ring_queue_cons examples/ring_queue_demo/cons.c)
target_link_libraries(ring_queue_cons -lrt)
install(TARGETS ring_queue_cons DESTINATION bin)


add_executable(ring_queue_cache_prod examples/ring_queue_cache_demo/prod.c)
target_link_libraries(ring_queue_cache_prod -lrt)
install(TARGETS ring_queue_cache_prod DESTINATION bin)

add_executable(ring_queue_cache_cons examples/ring_queue_cache_demo/cons.c)
target_link_libraries(ring_queue_cache_cons -lrt)
install(TARGETS ring_queue_cache_cons DESTINATION bin)


add_executable(calm_seq_lock_prod examples/calm_seq_lock_demo/prod.c)
target_link_libraries(calm_seq_lock_prod -lrt)
install(TARGETS calm_seq_lock_prod DESTINATION bin)

add_executable(calm_seq_lock_cons examples/calm_seq_lock_demo/cons.c)
target_link_libraries(calm_seq_lock_cons -lrt)
install(TARGETS calm_seq_lock_cons DESTINATION bin)
