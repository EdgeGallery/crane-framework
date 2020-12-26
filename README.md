# crane-framework

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) ![Jenkins](https://img.shields.io/jenkins/build?jobUrl=http%3A%2F%2Fjenkins.edgegallery.org%2Fview%2FMEC-PLATFORM-BUILD%2Fjob%2Fdeveloper-backend-docker-image-build-update-daily-master%2F)

crane-framework将可复用的计算和软件功能抽象成插件，APP开发者面向使用插件进行MEC APP开发。这样屏蔽了和MEC平台交互的细节，实现MCE APP和MEC平台的松耦合。而且插件框架基础能力可裁剪，按需提供最小的APP系统。

## 特性介绍

为了方便开发者进行APP的开发，我们提供crane插件框架，主要包含以下能力：

- **插件制作** 
    
    插件框架提供了单例插件和多例插件模型，开发者可以基于框架模型制作自己的插件，将自己的插件贡献到平台上。

- **插件管理** 
    
    插件框架帮助开发者进行快速开发，提供插件的注册、加载、卸载等能力，并且插件可持续更新。

- **系统插件** 
    
    插件框架向开发者提供实用的插件，目前提供的系统插件有mep、msg、httpserver。

## 编译运行

  crane-framework作为动态库对外提供，同时该框架支持gstreamer框架，开发者可以基于该框架进行二次开发。

- ### 环境准备
  
    |  Name     | Version   |
    |  ----     | ----  |
    | python3.8 |3.8xxx or above |
    | SWIG |3.0 |
    | rapidjson |v1.1.0 |
    | Openssl | 1.1.1 |
    | cpprestsdk  | 2.10.2 |
    | pistache  | VERSION_GIT_DATE 20200708 |
    | gtest  | 1.10.x |
    | curl  | 7.58.0 |
    | gstreamer  | 1.0 |

    三方件的安装可以参考[env-pre.sh](https://gitee.com/edgegallery/crane-framework/blob/master/crane-env-pre.sh)

- ### Crane框架和Crane插件、app源文件编译
1. cd src/..; mkdir build
2. cd build;
3. cmake -DCRANE_BUILD_TESTS=true -DCMAKE_BUILD_TYPE=Release ..
4. make
5. make test
6. 编译完成后，在build/lib和build/bin目录下生成相应的so文件和可执行文件

- ### Crane框架和Crane插件安装到/usr/local目录
1. sudo make install
2. sudo cmake --build . --target install-playcd-python
   sudo cmake --build . --target install-crane-python
3. 安装完成后，在/usr/local/include/crane、/usr/local/lib/crane目录下生成相应的h文件和插件框架、Crane系统插件的so文件

- ### Gstreamer插件编译
1. cd src/plugins/gst;
2. ./autogen.sh
3. make; make install
4. 编译、安装完成后，Gstreamer插件的so文件安装到/usr/local/lib/gstreamer-1.0

- ### Demo程序运行
1. cd src/../build/bin;
2. ./run_crane_plugin_create_and_destory.out
3. ./run_crane_plugin_create_singleton.out
4. ./run_crane_plugin_http_server.out&; curl -k -X POST "https://127.0.0.1:8080/api/v1/img/dongyin"
5. cd src/app/src/; python crane_plugin_python_interface.py