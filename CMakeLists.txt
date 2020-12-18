cmake_minimum_required (VERSION 3.10)

find_program(CMAKE_C_COMPILER NAMES gcc /usr/bin)
find_program(CMAKE_CXX_COMPILER NAMES g++ /usr/bin)

project(LibCraneFramework)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -pedantic -Wextra -Wno-missing-field-initializers")
set(CMAKE_INSTALL_PREFIX "/usr/local")

set(PYTHON_INCLUDE_DIR "/usr/include/python3.8")
set(PYTHON_LIBRARY "/usr/lib/python3.8/config-3.8-x86_64-linux-gnu/libpython3.8.so")

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -fstack-protector-all --param=ssp-buffer-size=4")
endif()

#构造IMG Demo的编译选项，控制代码中的BUILD_IMG宏
if (CRANE_BUILD_IMG)
    add_definitions(-DBUILD_IMG)
endif()

add_subdirectory(src/crane/common)
add_subdirectory(src/crane/craneCore)
add_subdirectory(src/crane/cranePlugins/msg)
add_subdirectory(src/crane/cranePlugins/mep)
add_subdirectory(src/crane/cranePlugins/srv)
#add_subdirectory(src/crane/craneMep)
#add_subdirectory(src/crane/craneServer)
add_subdirectory(src/crane/gstFacade)
add_subdirectory(src/crane/craneAdapter)
add_subdirectory(src/crane/craneWrapper)
add_subdirectory(src/crane)
add_subdirectory(src/crane/craneSwig)

#plugins
add_subdirectory(src/plugins/crane/playcd/cpp)
add_subdirectory(src/plugins/crane/playcd/wrap)
add_subdirectory(src/plugins/crane/playcd/swig)

add_subdirectory(src/plugins/crane/playmp3/cpp)

add_subdirectory(src/plugins/crane/fetchImgInfo)
add_subdirectory(src/plugins/crane/singleton)
add_subdirectory(src/app)

if (CRANE_BUILD_TESTS)
    find_package(GTest)
    if (GTEST_FOUND)
        include_directories($(GTEST_INCLUDE_DIRS))
        message(STATUS "GTest is found")
    else()
        message(STATUS "GTest is not found")
        add_subdirectory(third-party/googletest)
        include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
    endif()
        enable_testing()
        add_subdirectory(tests)
endif()

#cmake ../src/crane 这个命令的执行位置需要在build目录下
#set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

link_directories(${PROJECT_BINARY_DIR}/lib)

#####################################################
message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("CMAKE_INSTALL_FULL_LIBDIR: ${CMAKE_INSTALL_FULL_LIBDIR}")
message("CMAKE_INSTALL_FULL_INCLUDEDIR: ${CMAKE_INSTALL_FULL_INCLUDEDIR}")

#get_property(ABC GLOBAL PROPERTY "CRANE_H")

#####################################################
#target_link_libraries(libcrane common cranecore gstfacade craneadapter)
#target_link_libraries(libcrane -lcommon -lcranecore)
#target_link_libraries(LibCraneFramework libcommon libcranecore)

#在src/../build路径下执行`cmake -DCRANE_BUILD_TESTS=true ..`生成cmake相关文件；
#修改src/../build路径下CMakeCache.txt中CMAKE_CXX_COMPILER宏，将其指向需要使用的c++编译器可执行文件
#在src/../build路径下执行`make`完成编译
#动态库生成在build/lib/下的对应路径
#可执行文件生成在build/bin路径下

#make完成后，通过下面命令完成python的安装，安装路径在/usr/lib/python3/dist-packages下
#sudo cmake --build . --target install-playcd-python
#sudo cmake --build . --target install-crane-python
