# CMakeLists.txt
cmake_minimum_required (VERSION 3.5.1)
project (ipc)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

include_directories(include)

file(GLOB HEADERS "include/ipc/*.h")
install(FILES ${HEADERS} DESTINATION "include/ipc")


file(GLOB EXAMPLES "examples/*")

foreach(example ${EXAMPLES})
  execute_process(COMMAND bash "-c" "basename -z ${example}" OUTPUT_VARIABLE example_name)
  #message("example_name: ${example_name}")

  file(GLOB example_targets "${example}/*.c*")
  foreach(target ${example_targets})
    execute_process(COMMAND bash "-c" "basename -z -s .cpp ${target} | cut -z -f 1 -d '.'" OUTPUT_VARIABLE target_name)
    #message("${target}: ${target_name}")
    set(example_target "${example_name}_${target_name}")

    add_executable(${example_target} EXCLUDE_FROM_ALL ${target})
    target_link_libraries(${example_target} -lrt)
    install(TARGETS ${example_target} DESTINATION bin OPTIONAL)

    set(EXAMPLE_NAMES "${EXAMPLE_NAMES}${example_target};")
    set(EXAMPLE_FILES "${EXAMPLE_FILES}${target};")
  endforeach()

endforeach()
#message("example names: ${EXAMPLE_NAMES}")
#message("example files: ${EXAMPLE_FILES}")

#add_custom_target(examples ALL)
#add_custom_target(examples DEPENDS ${HEADERS})
add_custom_target(examples)
add_dependencies(examples ${EXAMPLE_NAMES})
