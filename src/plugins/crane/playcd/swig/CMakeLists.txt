
FIND_PACKAGE(SWIG REQUIRED)

INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)

link_libraries(stdc++)

#set(CMAKE_CXX_FLAGS "-Wall -Wconversion -pedantic -Wextra -Wno-missing-field-initializers -O3 -DNDEBUG -fPIC -std=gnu++14")

include_directories(${PYTHON_INCLUDE_PATH})
include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/cpp/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/plugins/crane/playcd/wrap/inc")

#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/plugins")
set(CMAKE_SWIG_FLAGS "")

message("CMAKE_CXX_FLAGS:=============== ${CMAKE_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")
message("CMAKE_CXX_FLAGS:=============== ${CMAKE_CXX_FLAGS}")

set_source_files_properties(playcd.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(playcd.i PROPERTIES SWIG_FLAGS "-includeall")

swig_add_library(craneplaycd LANGUAGE python SOURCES playcd.i)

swig_link_libraries(craneplaycd wrapcraneplayercd stdc++ ${PYTHON_LIBRARYIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/craneplaycd.py
        ${CMAKE_CURRENT_BINARY_DIR}/_craneplaycd.so)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Declare install target for python
#install(TARGET _craneplaycd
#        COMMAND "python setup.py install"
#       COMPONENT swig-python)

# Install target to call setup.py
add_custom_target(install-playcd-python
        DEPENDS _craneplaycd
        COMMAND python ${SETUP_PY_OUT} install)