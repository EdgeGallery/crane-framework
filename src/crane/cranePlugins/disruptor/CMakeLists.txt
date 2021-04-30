set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")

include_directories("${PROJECT_SOURCE_DIR}/third-party/disruptor")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/disruptor/inc")

aux_source_directory("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/disruptor/src" libcranedisruptorplugin_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane/plugins")

add_library(cranedisruptor OBJECT ${libcranedisruptorplugin_src})