set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/gstFacade/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneCore/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneMep/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneServer/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/fetchImgInfo/inc")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/crane/craneServer/src libCraneServer_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

add_library(craneserver OBJECT ${libCraneServer_src})