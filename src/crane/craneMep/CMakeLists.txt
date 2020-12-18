set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src/crane/craneMep/inc")
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/crane/craneMep/src libCraneMep_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

add_library(cranemep OBJECT ${libCraneMep_src})