import os
import sys
import math

import bpy
import mathutils

sys.path.append(".")
import oogl

Iblender = "blue.blend"
Camera = "data/elliptocyte/00001.camera"
Geometry = "data/elliptocyte/00001.geometry"
Oblender = "o.blend"
Opng = "o.png"
wx = 800
wy = 600

fov = oogl.fov(Camera)
fov *= 2*math.pi/180
bpy.ops.wm.open_mainfile(filepath = Iblender)

mesh = oogl.off(Geometry)
cam = bpy.data.objects['camera']
scene = bpy.data.scenes["Scene"]
scene.render.resolution_x = wx
scene.render.resolution_y = wy

cam.data.angle = fov
M = oogl.transform(Camera)
cam.matrix_world = M

cell = bpy.data.objects['cell']
cell.data = mesh
cell.active_material = bpy.data.materials['Cell']
M = oogl.transform(Geometry)
cell.matrix_world = M

bpy.ops.wm.save_as_mainfile(filepath = Oblender)

bpy.data.scenes['Scene'].render.filepath = Opng
bpy.ops.render.render(write_still = True)
