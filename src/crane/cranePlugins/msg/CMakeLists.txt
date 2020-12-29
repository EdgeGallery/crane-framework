set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/inc")
aux_source_directory("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/src" libcranemsgplugin_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane/plugins")

add_library(cranemsg OBJECT ${libcranemsgplugin_src})
