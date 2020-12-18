'''
Descripttion: 
Version: 1.0
Author: dongyin@huawei.com
Date: 2020-09-29 14:52:02
LastEditors: dongyin@huawei.com
LastEditTime: 2020-10-04 17:39:32
'''
import crane4py
import craneplaycd
import os
import time

print('start...')

crane4py.init(2)

plugin = crane4py.create('Itf_Player', 'PlayerImplCD', 'my first plugin instance')
craneplaycd.playcd_play(plugin, 'dongyin')
crane4py.destory(plugin)

print('===============================unload PlayerImplCD plugin======================================')
crane4py.unload('Itf_Player', 'PlayerImplCD')

print('===============================load PlayerImplCD plugin again======================================')
crane4py.load(os.path.abspath('../../../build/lib/plugins/libcranepluginplayercd.so'))
plugin = crane4py.create('Itf_Player', 'PlayerImplCD', 'my first plugin instance')
craneplaycd.playcd_play(plugin, 'dongyin')
crane4py.destory(plugin)
crane4py.unload('Itf_Player', 'PlayerImplCD')


filesrc = crane4py.create('filesrc', 'my_filesource')
print (filesrc)