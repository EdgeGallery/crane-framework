FIND_PACKAGE(SWIG REQUIRED)

INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)

link_libraries(stdc++)

include_directories(${PYTHON_INCLUDE_PATH})
include_directories("${PROJECT_SOURCE_DIR}/src/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/common/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneServer/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneWrapper/inc")
include_directories("${PROJECT_SOURCE_DIR}/src/crane/craneAdapter/inc")

set(CMAKE_SWIG_FLAGS "")

set_source_files_properties(crane4py.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(crane4py.i PROPERTIES SWIG_FLAGS "-includeall")

swig_add_library(crane4py LANGUAGE python SOURCES crane4py.i)
swig_link_libraries(crane4py crane stdc++ ${PYTHON_LIBRARYIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/crane4py.py
        ${CMAKE_CURRENT_BINARY_DIR}/_crane4py.so)

# Configure setup.py and copy to output directory
set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in)
set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

# Declare install target for python
#install(TARGETS swig_example
#        COMMAND "${PYTHON_EXECUTABLE} setup.py"
#        COMPONENT swig-python)

# Install target to call setup.py
add_custom_target(install-crane-python
        DEPENDS _crane4py
        COMMAND python ${SETUP_PY_OUT} install)
