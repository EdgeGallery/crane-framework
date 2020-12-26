#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")

#execute_process(COMMAND pkg-config --cflags gstreamer-1.0
#                OUTPUT_VARIABLE CMAKE_C_FLAGS)
#string(REPLACE "\n" "" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
#message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
#
#execute_process(COMMAND pkg-config --libs gstreamer-1.0
#                OUTPUT_VARIABLE CMAKE_EXE_LINKER_FLAGS)
#string(REPLACE "\n" "" CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS})
#message("CMAKE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")



include_directories("${PROJECT_SOURCE_DIR}/src/inc")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneCore/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/gstFacade/inc")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/mep/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/srv/inc")

include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/singleton/inc")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/app/src app_src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

link_directories("${PROJECT_BINARY_DIR}/lib/crane")

#链接静态库
link_libraries(stdc++)

function(crane_example example_name)
    set(EXAMPLE_EXECUTABLE run_${example_name}.out)
    set(EXAMPLE_SOURCE src/${example_name}.cpp)

    add_executable(${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_EXECUTABLE} crane dl boost_system crypto ssl cpprest)
endfunction()

crane_example(crane_plugin_create_and_destory)
crane_example(crane_plugin_create_singleton)
crane_example(crane_plugin_http_server)
