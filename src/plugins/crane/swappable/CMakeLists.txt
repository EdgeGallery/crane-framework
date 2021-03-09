set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/car/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/trunck/inc")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/car/src libpluginscranecar_src)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/plugins/crane/swappable/trunck/src libpluginscranetrunck_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")

add_library(craneplugincar MODULE ${libpluginscranecar_src})
add_library(craneplugincar_static STATIC ${libpluginscranecar_src})

add_library(craneplugintrunck MODULE ${libpluginscranetrunck_src})
add_library(craneplugintrunck_static STATIC ${libpluginscranetrunck_src})