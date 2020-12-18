set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -shared -fPIC")

include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/gstFacade/inc")
aux_source_directory(${PROJECT_SOURCE_DIR}/src/crane/gstFacade/src libgstFacade_src)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/crane")

add_library(gstfacade OBJECT ${libgstFacade_src})
#add_library(gstfacade SHARED ${libgstFacade_src})
