'''
Descripttion: 
Version: 1.0
Author: dongyin@huawei.com
Date: 2020-09-29 14:52:02
LastEditors: dongyin@huawei.com
LastEditTime: 2020-10-21 17:08:51
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