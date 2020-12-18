set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/fetchImgInfo/inc")
aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/fetchImgInfo/src libpluginscranefetchimginfo_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")

add_library(cranepluginfetchimginfo MODULE ${libpluginscranefetchimginfo_src})
target_link_libraries(cranepluginfetchimginfo python3.8)
