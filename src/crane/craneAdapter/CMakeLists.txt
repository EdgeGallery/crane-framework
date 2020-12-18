set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

file(GLOB _CRANEADAPTER_H_ ${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc/*.h)
get_property(TMP GLOBAL PROPERTY "CRANE_H")
set_property(GLOBAL PROPERTY CRANE_H "${TMP} ${_CRANEADAPTER_H_}")
get_property(ABC GLOBAL PROPERTY "CRANE_H")
message("CRANE_H: ${ABC}")

include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/gstFacade/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneCore/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneMep/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneServer/inc")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/cranePlugins/msg/inc")

aux_source_directory(${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/src libCraneAdapter_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

#link_directories(${LIBRARY_OUTPUT_PATH}/lib)

#dongyin
add_library(craneadapter OBJECT ${libCraneAdapter_src})
#add_library(craneadapter SHARED ${libCraneAdapter_src})
