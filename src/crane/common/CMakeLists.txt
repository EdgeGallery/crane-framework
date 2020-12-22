#message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")
#message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

file(GLOB _COMMON_H_ ${PROJECT_SOURCE_DIR}/src/crane/common/inc/*.h)
get_property(TMP GLOBAL PROPERTY "CRANE_H")
set_property(GLOBAL PROPERTY CRANE_H "${TMP} ${_COMMON_H_}")
get_property(ABC GLOBAL PROPERTY "CRANE_H")
#message("CRANE_H: ${ABC}")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
aux_source_directory(${PROJECT_SOURCE_DIR}/src/crane/common/src libcommon_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")
#link_directories(${CMAKE_SOURCE_DIR}/ext/jsoncpp/lib)

add_library(common OBJECT ${libcommon_src})
#add_library(common SHARED ${libcommon_src})
