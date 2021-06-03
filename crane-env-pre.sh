#!/bin/sh
# Copyright 2020 Huawei Technologies Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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

    #Copy to system directory，note，If the location of the download directory changes in different versions，usefind . -name "libgtest*.a" Find location
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
    #Create soft connection
    ln -s /usr/local/lib/libpistache-0.0.002-git20200802.so /usr/local/lib/libpistache.so
    cd ../../
}

echo "准备环境"
sudo apt-get update

echo "安装cmake"
sudo apt-get install -y cmake

echo "安装gtest"
install_gtest 
#sudo apt-get install googletest useadp-getMissing related header files after installation

echo "安装python3.8"
sudo apt-get install -y python3.8-dev

echo "安装SWIG"
sudo apt-get install -y swig

echo "安装rapidjson"
sudo apt-get install -y rapidjson-dev

#installationopenssl，目前环境基本都默认installation的有，如果没有可以使用如下命令installation
#sudo apt-get install -y openssl

echo "安装cpprestsdk"
sudo apt-get install -y libcpprest-dev

echo "安装curl"
sudo apt-get install -y libcurl4-openssl-dev

echo "安装pistache"
install_pistache

echo "安装Gstreamer"
install_gstreamer


