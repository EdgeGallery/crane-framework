## Crane框架和Crane插件、app源文件编译
1. cd src/..; mkdir build
2. cd build;
3. cmake -DCRANE_BUILD_TESTS=true -DCMAKE_BUILD_TYPE=Release ..
4. make
5. make test
6. 编译完成后，在build/lib和build/bin目录下生成相应的so文件和可执行文件

## Crane框架和Crane插件安装到/usr/local目录
1. sudo make install
2. sudo cmake --build . --target install-playcd-python
   sudo cmake --build . --target install-crane-python
3. 安装完成后，在/usr/local/include/crane、/usr/local/lib/crane目录下生成相应的h文件和插件框架、Crane系统插件的so文件

## Gstreamer插件编译
1. cd src/plugins/gst;
2. ./autogen.sh
3. make; make install
4. 编译、安装完成后，Gstreamer插件的so文件安装到/usr/local/lib/gstreamer-1.0

## Demo程序运行
1. cd src/../build/bin;
2. ./run_crane_plugin_create_and_destory.out
3. ./run_crane_plugin_create_singleton.out
4. ./run_crane_plugin_http_server.out&; curl -k -X POST "https://127.0.0.1:8080/api/v1/img/dongyin"
5. cd src/app/src/; python crane_plugin_python_interface.py