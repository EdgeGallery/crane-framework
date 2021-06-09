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
    echo "step 1: downloading"
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gstreamer.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-base.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-good.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-bad.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-plugins-ugly.git
    git clone -b 1.16.2 https://gitlab.freedesktop.org/gstreamer/gst-libav.git

    echo "step 2: installing dependency"
    sudo apt-get install -y autoconf
    sudo apt-get install -y autopoint
    sudo apt-get install -y bison
    sudo apt-get install -y flex
    sudo apt-get install -y glib2.0
    sudo apt-get install -y yasm
    sudo apt-get install -y libgtk-3-dev
    sudo apt-get install -y libssl-dev

    echo "step 3: switch the verison to gcc5 and g++5"
    sudo apt-get install -y gcc-5
    sudo apt-get install -y g++-5
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 --slave /usr/bin/g++ g++ /usr/bin/g++-5
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 50 --slave /usr/bin/g++ g++ /usr/bin/g++-7

    echo "step 4: building and installing in every directory"
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

    #Copy to system directory，note，If the location of the download directory changes in different versions，use command 'find . -name "libgtest*.a"' to find location
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

echo "preparing the enviroment"
sudo apt-get update

echo "installing cmake"
sudo apt-get install -y cmake

echo "installing gtest"
install_gtest 
#sudo apt-get install googletest   Missing related header files after using adp-get installation

echo "installing python3.8"
sudo apt-get install -y python3.8-dev

echo "installing SWIG"
sudo apt-get install -y swig

echo "installing rapidjson"
sudo apt-get install -y rapidjson-dev

#please use below command to install openssl, if environment does not have it already
#sudo apt-get install -y openssl

echo "installing cpprestsdk"
sudo apt-get install -y libcpprest-dev

echo "installing curl"
sudo apt-get install -y libcurl4-openssl-dev

echo "installing pistache"
install_pistache

echo "installing Gstreamer"
install_gstreamer


