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


add_executable(big_seq_lock_prod examples/big_seq_lock_demo/prod.c)
target_link_libraries(big_seq_lock_prod -lrt)
install(TARGETS big_seq_lock_prod DESTINATION bin)

add_executable(big_seq_lock_cons examples/big_seq_lock_demo/cons.c)
target_link_libraries(big_seq_lock_cons -lrt)
install(TARGETS big_seq_lock_cons DESTINATION bin)


add_executable(seq_lock_prod examples/seq_lock_demo/prod.c)
target_link_libraries(seq_lock_prod -lrt)
install(TARGETS seq_lock_prod DESTINATION bin)

add_executable(seq_lock_cons examples/seq_lock_demo/cons.c)
target_link_libraries(seq_lock_cons -lrt)
install(TARGETS seq_lock_cons DESTINATION bin)


add_executable(cpp_seq_lock_prod examples/cpp_seq_lock_demo/prod.cpp)
target_link_libraries(cpp_seq_lock_prod -lrt)
install(TARGETS cpp_seq_lock_prod DESTINATION bin)

add_executable(cpp_seq_lock_cons examples/cpp_seq_lock_demo/cons.cpp)
target_link_libraries(cpp_seq_lock_cons -lrt)
install(TARGETS cpp_seq_lock_cons DESTINATION bin)


add_executable(cpp_big_seq_lock_prod examples/cpp_big_seq_lock_demo/prod.cpp)
target_link_libraries(cpp_big_seq_lock_prod -lrt)
install(TARGETS cpp_big_seq_lock_prod DESTINATION bin)

add_executable(cpp_big_seq_lock_cons examples/cpp_big_seq_lock_demo/cons.cpp)
target_link_libraries(cpp_big_seq_lock_cons -lrt)
install(TARGETS cpp_big_seq_lock_cons DESTINATION bin)



add_executable(cpp_ipc_prod examples/cpp_ipc_demo/prod.cpp)
target_link_libraries(cpp_ipc_prod -lrt)
install(TARGETS cpp_ipc_prod DESTINATION bin)

add_executable(cpp_ipc_cons examples/cpp_ipc_demo/cons.cpp)
target_link_libraries(cpp_ipc_cons -lrt)
install(TARGETS cpp_ipc_cons DESTINATION bin)
