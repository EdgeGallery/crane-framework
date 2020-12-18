set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/craneCore/inc")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/crane/craneCore/src libCraneCore_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

add_library(cranecore OBJECT ${libCraneCore_src})
#add_library(cranecore SHARED ${libCraneCore_src})
