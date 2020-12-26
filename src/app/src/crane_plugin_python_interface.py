'''
     Copyright 2020 Huawei Technologies Co., Ltd.
 
     Licensed under the Apache License, Version 2.0 (the "License");
     you may not use this file except in compliance with the License.
     You may obtain a copy of the License at
 
         http://www.apache.org/licenses/LICENSE-2.0
 
     Unless required by applicable law or agreed to in writing, software
     distributed under the License is distributed on an "AS IS" BASIS,
     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
     See the License for the specific language governing permissions and
     limitations under the License.
'''
import crane4py
import craneplaycd
import os

print('===================Initialize Crane and Gstreamer========================')
crane4py.init(2)

print('==================Create Crane plugin====================================')
plugin = crane4py.create('Itf_Player', 'PlayerImplCD', 'my first plugin instance')
craneplaycd.playcd_play(plugin, 'dongyin')
crane4py.destory(plugin)

print('==================unload PlayerImplCD plugin=============================')
crane4py.unload('Itf_Player', 'PlayerImplCD')

print('==================load PlayerImplCD plugin again=========================')
crane4py.load(os.path.abspath('../../../build/lib/plugins/libcranepluginplayercd.so'))
plugin = crane4py.create('Itf_Player', 'PlayerImplCD', 'my first plugin instance')
craneplaycd.playcd_play(plugin, 'dongyin')
crane4py.destory(plugin)
crane4py.unload('Itf_Player', 'PlayerImplCD')

print('==================Create Gstreamer plugin================================')
filesrc = crane4py.create('filesrc', 'my_filesource')
print (filesrc)