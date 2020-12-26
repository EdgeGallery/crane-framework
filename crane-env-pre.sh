#!/bin/bash

install_gstreamer()
{
    echo "步骤一：下载"
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gstreamer.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-base.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-good.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-bad.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-ugly.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-libav.git

    echo "步骤二：安装依赖"
    sudo apt-get install -y autoconf
    sudo apt-get install -y autopoint
    sudo apt-get install -y bison
    sudo apt-get install -y flex
    sudo apt-get install -y glib2.0
    sudo apt-get install -y yasm
    sudo apt-get install -y libgtk-3-dev
    sudo apt-get install -y libssl-dev

    echo "步骤三：将版本切换到gcc5和g++5"
    sudo apt-get install -y gcc-5
    sudo apt-get install -y g++-5
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 --slave /usr/bin/g++ g++ /usr/bin/g++-5
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 50 --slave /usr/bin/g++ g++ /usr/bin/g++-7

    echo "步骤四：在各个目录下执行编译、安装"
    cd gstreamer
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../gst-plugins-base/
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../gst-plugins-good/
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../gst-plugins-bad/
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../gst-plugins-ugly/
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../gst-libav/
    ./autogen.sh --disable-gtk-doc
    make
    sudo make install

    cd ../
}

install_gtest()
{
    git clone https://github.com/google/googletest
    cd googletest
    cmake CMakeLists.txt
    make

    #拷贝到系统目录，注意，如果下诉目录位置在不同版本位置有变动，用find . -name "libgtest*.a" 找到位置
    sudo cp lib/libgtest*.a  /usr/lib
    sudo cp -a googletest/include/gtest /usr/include
    cd ../
}

install_pistache()
{
    git clone https://github.com/pistacheio/pistache
    git submodule update --init
    cd pistache;mkdir build
    cd build
    cmake -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DPISTACHE_BUILD_EXAMPLES=true \
        -DPISTACHE_BUILD_TESTS=true \
        -DPISTACHE_BUILD_DOCS=false \
        -DPISTACHE_USE_SSL=true \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        ..
    make
    #make test
    sudo make install
    #创建软连接
    ln -s /usr/local/lib/libpistache-0.0.002-git20200802.so /usr/local/lib/libpistache.so
    cd ../../
}

echo "准备环境"
sudo apt-get update

echo "安装cmake"
sudo apt-get install -y cmake

echo "安装gtest"
install_gtest 
#sudo apt-get install googletest 使用adp-get安装后缺少相关头文件

echo "安装python3.8"
sudo apt-get install -y python3.8-dev

echo "安装SWIG"
sudo apt-get install -y swig

echo "安装rapidjson"
sudo apt-get install -y rapidjson-dev

#安装openssl，目前环境基本都默认安装的有，如果没有可以使用如下命令安装
#sudo apt-get install -y openssl

echo "安装cpprestsdk"
sudo apt-get install -y libcpprest-dev

echo "安装curl"
sudo apt-get install -y libcurl4-openssl-dev

echo "安装pistache"
install_pistache

echo "安装Gstreamer"
install_gstreamer


