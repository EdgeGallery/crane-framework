message("CMAKE_CXX_FLAGS of playcd: ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playmp3/cpp/inc")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/playmp3/cpp/src libpluginscraneplaymp3_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")

add_library(cranepluginplayermp3 MODULE ${libpluginscraneplaymp3_src})
add_library(cranepluginplayermp3_static STATIC ${libpluginscraneplaymp3_src})