## 环境配置
```shell
cmake 3.10.2
```
```shell
apt-get update
```
### Gstreamer 安装
#### 步骤一：下载
```shell
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gstreamer.git
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-base.git
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-good.git
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-bad.git
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-ugly.git
git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-libav.git
```
#### 步骤二：安装依赖
sudo apt-get install -y autoconf
sudo apt-get install -y autopoint
sudo apt-get install -y bison
sudo apt-get install -y flex
sudo apt-get install -y glib2.0
sudo apt-get install -y yasm
sudo apt-get install -y libgtk-3-dev
sudo apt-get install -y libssl-dev

#### 步骤三：将版本切换到gcc5和g++5
sudo apt-get install -y gcc-5
sudo apt-get install -y g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 --slave /usr/bin/g++ g++ /usr/bin/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 50 --slave /usr/bin/g++ g++ /usr/bin/g++-7
sudo update-alternatives --config gcc

#### 步骤四：在各个目录下执行编译、安装
./autogen.sh --disable-gtk-doc

Configuration
        Version                    : 1.16.2
        Source code location       : .
        Prefix                     : /usr/local
        Compiler                   : gcc
        Package name               : GStreamer source release
        Package origin             : Unknown package origin

        API Documentation          : no

        Debug logging              : yes
        Tracing subsystem hooks    : yes
        Command-line parser        : yes
        Option parsing in gst_init : yes
        Plugin registry            : yes
        Plugin support             : yes
        Static plugins             :
        Unit testing support       : yes
        PTP clock support          : yes
        libunwind support          : no
        libdw support              : no

        Debug                      : yes
        Profiling                  : no

        Building benchmarks        : yes
        Building examples          : yes
        Building test apps         : yes
        Building tests that fail   : yes
        Building tools             : yes

Now type 'make' to compile gstreamer.


make -j
sudo make install


Libraries have been installed in:
   /usr/local/lib/gstreamer-1.0

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the '-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the 'LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the 'LD_RUN_PATH' environment variable
     during linking
   - use the '-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to '/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.

### 安装cmake 3.10.2及以上版本
sudo apt-get install -y cmake

### 其他开源项目依赖
1. python3.8
2. SWIG 3.0
3. rapidjson v 1.1.0
4. pistache VERSION_GIT_DATE 20200708
5. Openssl 1.1.1
6. cpprestsdk 2.10.2

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