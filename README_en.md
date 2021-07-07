# crane-framework

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) ![Jenkins](https://img.shields.io/jenkins/build?jobUrl=http%3A%2F%2Fjenkins.edgegallery.org%2Fview%2FMEC-PLATFORM-BUILD%2Fjob%2Fdeveloper-backend-docker-image-build-update-daily-master%2F)

crane-frameworkAbstract reusable computing and software functions into plug-ins，APPDevelopers are oriented towards using pluginsMEC APPDevelopment。So blocked andMECDetails of platform interaction，achieveMEC APPwithMECLoose coupling of the platform。And the basic capabilities of the plug-in framework can be tailored，Provide the smallest on demandAPPsystem。

## Features Introduction

For the convenience of developersAPPDevelopment，we providecranePlug-in framework，The framework can support multiple language development（Currently supportedC++、Python），Mainly include the following capabilities：

- **Plug-in production** 
    
    The plug-in framework provides singleton plug-in and multi-case plug-in models，Developers can make their own plug-ins based on the framework model，Contribute your own plugin to the platform。

- **Plug-in management** 
    
    Plug-in framework helps developers to develop quickly，Provide plug-in registration、load、Unloading and other capabilities，And the plugin can be updated continuously。

- **System plug-in** 
    
    The plug-in framework provides developers with useful plug-ins，The system plug-ins currently provided aremep、msg、httpserver。

## Compile and run

  crane-frameworkProvided as a dynamic library，At the same time, the framework supportsgstreamerframe，开发者可以基于该frame进行二次开发。

- ### Environmental preparation
  
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

    The installation of the tripartite can refer to[env-pre.sh](https://gitee.com/edgegallery/crane-framework/blob/master/crane-env-pre.sh)

- ### CraneFrame andCranePlug-in、appSource file compilation
1. cd src/..; mkdir build
2. cd build;
3. cmake -DCRANE_BUILD_TESTS=true -DCMAKE_BUILD_TYPE=Release ..
4. make
5. Before testing，Need to be testedcraneCopy the relevant configuration files to the working directory/rootunder，And set environment variables（Pay attention to modify the actual code path in the sample）
   ```
   cp ../tests/crane-cnf/.crane.cnf ../tests/crane-cnf/.crane_cache ../tests/crane-cnf/.crane_plugin_mep.cnf ../tests/crane-cnf/.crane_plugin_srv.cnf /root
   export MEP_APIGW_ENDPOINT=127.0.0.1:8443
   export MEP_APIGW_NOTIFY_URL=127.0.0.1:9090
   export APP_INSTANCE_ID=682a2fb92b224029b0d02205231b8450
   export CRANE_SYS_PLUGIN_PATH=/home/lxy/crane-framework/build/lib/crane
   export CRANE_APP_PLUGIN_PATH=/home/lxy/crane-framework/build/lib/plugins
   export CRANE_INIT_MODE=LIB
   ```
6. make test
7. After compilation，inbuild/libwithbuild/binGenerate the correspondingso文件with可执行文件

- ### CraneFrame andCraneThe plugin is installed to/usr/localtable of Contents
1. sudo make install
2. sudo cmake --build . --target install-playcd-python
   sudo cmake --build . --target install-crane-python
3. After the installation is complete，in/usr/local/include/crane、/usr/local/lib/craneGenerate the correspondinghFile and plugin framework、CraneSystem plug-insofile

- ### GstreamerPlugin compilation
1. cd src/plugins/gst;
2. ./autogen.sh
3. make; make install
4. Compile、After the installation is complete，GstreamerPlug-insoFile installed to/usr/local/lib/gstreamer-1.0

- ### DemoProgram running
1. cd src/../build/bin;
2. ./run_crane_plugin_create_and_destory.out
3. ./run_crane_plugin_create_singleton.out
4. ./run_crane_plugin_http_server.out&; curl -k -X POST "https://127.0.0.1:8080/api/v1/img/dongyin"
5. cd src/app/src/; python crane_plugin_python_interface.py