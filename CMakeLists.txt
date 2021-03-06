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

#关闭Wrapper插件包装类中和直接创建插件相关的构造函数 和 与之相关的测试代码。
if (CRANE_BUILD_WRAPPER_CONSTRUCTOR)
    add_definitions(-DBUILD_WRAPPER_CONSTRUCTOR)
endif()

link_directories("/usr/local/lib")
include_directories("/usr/include/glib-2.0")
include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include")
link_directories("/usr/lib/glib2.0")
include_directories("/usr/local/include/gstreamer-1.0")
link_directories("/usr/local/lib/gstreamer-1.0")
include_directories("/usr/include/python3.8")

link_directories(${PROJECT_BINARY_DIR}/third-party/disruptor/Disruptor)

#Third party dependence
add_subdirectory(third-party/disruptor)

add_subdirectory(src/crane/common)
add_subdirectory(src/crane/craneCore)
add_subdirectory(src/crane/cranePlugins/msg)
add_subdirectory(src/crane/cranePlugins/mep)
add_subdirectory(src/crane/cranePlugins/srv)
add_subdirectory(src/crane/cranePlugins/disruptor)
add_subdirectory(src/crane/gstFacade)
add_subdirectory(src/crane/craneAdapter)
add_subdirectory(src/crane/craneWrapper)
add_subdirectory(src/crane)
add_subdirectory(src/crane/craneSwig)

#application plugins
add_subdirectory(src/plugins/crane/playcd/cpp)
add_subdirectory(src/plugins/crane/playcd/wrap)
add_subdirectory(src/plugins/crane/playcd/swig)

add_subdirectory(src/plugins/crane/playmp3/cpp)

add_subdirectory(src/plugins/crane/fetchImgInfo)

add_subdirectory(src/plugins/crane/singleton)

add_subdirectory(src/plugins/crane/swappable)

#application example 
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
message("-- Install path: ${CMAKE_INSTALL_PREFIX}")
message("CMAKE_INSTALL_FULL_LIBDIR: ${CMAKE_INSTALL_FULL_LIBDIR}")
message("CMAKE_INSTALL_FULL_INCLUDEDIR: ${CMAKE_INSTALL_FULL_INCLUDEDIR}")

#get_property(ABC GLOBAL PROPERTY "CRANE_H")

#####################################################
#target_link_libraries(libcrane common cranecore gstfacade craneadapter)
#target_link_libraries(libcrane -lcommon -lcranecore)
#target_link_libraries(LibCraneFramework libcommon libcranecore)

#在src/../build路径下执行`cmake -DCRANE_BUILD_TESTS=true -DCRANE_IMG_BUILD=false -DCMAKE_BUILD_TYPE=Release ..`生成cmake相关文件；
#修改src/../build路径下CMakeCache.txt中CMAKE_CXX_COMPILER宏，将其指向需要使用的c++编译器可执行文件
#在src/../build路径下执行`make`完成编译
#动态库生成在build/lib/下的对应路径
#可执行文件生成在build/bin路径下
#在src/../build路径下执行sudo make install 安装crane框架和crane系统插件的头文件和库到/usr/local/对应的路径下

#make完成后，通过下面命令完成python包装后的so和py文件安装，安装目标路径在/usr/lib/python3/dist-packages下
#sudo cmake --build . --target install-playcd-python
#sudo cmake --build . --target install-crane-python
