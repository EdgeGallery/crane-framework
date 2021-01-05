#message("CMAKE_CXX_FLAGS of playcd: ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/src libpluginscraneplaycd_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")

add_library(cranepluginplayercd MODULE ${libpluginscraneplaycd_src})
add_library(cranepluginplayercd_static STATIC ${libpluginscraneplaycd_src})
