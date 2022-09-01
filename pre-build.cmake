message("Compiling shaders...")
file(GLOB SHADERS ${CMAKE_CURRENT_LIST_DIR}/src/engine/assets/shaders/*.frag ${CMAKE_CURRENT_LIST_DIR}/src/engine/assets/shaders/*.vert)
foreach(shader ${SHADERS})
    execute_process(COMMAND glslc -c ${shader} -o ${shader}.spv)
endforeach()

message("Copying assets to binary folder...")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/src/engine/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
