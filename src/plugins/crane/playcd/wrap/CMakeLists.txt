set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/wrap/inc")


aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/wrap/src libwrappluginscraneplaycd_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")

add_library(wrapcraneplayercd SHARED ${libwrappluginscraneplaycd_src})

target_link_libraries(wrapcraneplayercd cranepluginplayercd_static)
