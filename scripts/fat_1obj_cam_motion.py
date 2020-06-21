import pybullet as p
import pybullet_data
import time

import visii
import numpy as np 
from PIL import Image 
import PIL
import randomcolor
from utils import * 
import argparse

parser = argparse.ArgumentParser()
   
parser.add_argument('--spp', 
                    default=30,
                    type=int)
parser.add_argument('--width', 
                    default=500,
                    type=int)
parser.add_argument('--height', 
                    default=500,
                    type=int)
parser.add_argument('--noise',
                    action='store_true',
                    default=False)
parser.add_argument('--outf',
                    default='out_physics')
opt = parser.parse_args()


try:
    os.mkdir(opt.outf)
    print(f'created {opt.outf}/ folder')
except:
    print(f'{opt.outf}/ exists')




visii.initialize_headless()

if not opt.noise is True: 
    visii.enable_denoiser()


camera_entity = visii.entity.create(
    name="camera",
    transform=visii.transform.create("camera"),
    camera=visii.camera.create_perspective_from_fov(name = "camera", 
        field_of_view = 0.785398, 
        aspect = opt.width/float(opt.height),
        near = .1))



visii.set_camera_entity(camera_entity)
# camera_entity.get_transform().set_position(0, 0.0, -5.)
camera_entity.get_transform().set_position(visii.vec3(7,0,2))
camera_entity.get_transform().look_at(
    visii.vec3(0,0,0),
    visii.vec3(0,0,1),
    # visii.vec3(5,0,2),
)

print(camera_entity.get_transform().get_position())

light = visii.entity.create(
    name="light",
    mesh = visii.mesh.create_plane("light"),
    transform = visii.transform.create("light"),
    material = visii.material.create("light"),
    light = visii.light.create('light')
)
light.get_light().set_intensity(10000)
light.get_light().set_temperature(5000)
# light.get_transform().set_position(0,0,-0.1)
light.get_transform().set_position(visii.vec3(-3,1,4))
light.get_transform().set_scale(visii.vec3(0.4))
light.get_transform().set_rotation(visii.quat(0,0,1,0))

light2 = visii.entity.create(
    name="light2",
    mesh = visii.mesh.create_plane("light2"),
    transform = visii.transform.create("light2"),
    material = visii.material.create("light2"),
    light = visii.light.create('light2')
)
light2.get_light().set_intensity(1000)
light2.get_light().set_temperature(5000)
# light2.get_transform().set_position(0,0,-0.1)
light2.get_transform().set_position(visii.vec3(3,-1,4))
light2.get_transform().set_scale(visii.vec3(0.4))
light2.get_transform().set_rotation(visii.quat(0,0,1,0))
light2.get_light().set_color(visii.vec3(1,0,0))


dome = visii.texture.create_from_image("dome", "textures/abandoned_tank_farm_01_1k.hdr")
visii.set_dome_light_texture(dome)
visii.set_dome_light_intensity(0.1)
# Physics init 
physicsClient = p.connect(p.DIRECT) # or p.GUI or p.DIRECT for non-graphical version
p.setGravity(0,0,-10)


floor = visii.entity.create(
    name="floor",
    mesh = visii.mesh.create_plane("floor"),
    transform = visii.transform.create("floor"),
    material = visii.material.create("floor")
)
# floor.get_transform().set_position(0,0,-0.1)
floor.get_transform().set_position(visii.vec3(0,0,0))
floor.get_transform().set_scale(visii.vec3(10))
floor.get_material().set_roughness(1.0)

perlin = visii.texture.create_from_image("perlin", "tex.png")
floor.get_material().set_roughness_texture(perlin)

# random_material("floor")
floor.get_material().set_transmission(0)
floor.get_material().set_metallic(1.0)
floor.get_material().set_roughness(0)

floor.get_material().set_base_color(visii.vec3(0.5,0.5,0.5))

# Set the collision of the floor
plane_col_id = p.createCollisionShape(p.GEOM_PLANE)
p.createMultiBody(baseCollisionShapeIndex = plane_col_id,
                    basePosition = [0,0,0],
                    # baseOrientation= p.getQuaternionFromEuler([0,0,0])
                    )

# cube_visii = add_random_obj(name='cube',obj_id=3) # force to create a cube

# LOAD SOME OBJECTS 


import glob 

objects_dict = {}

base_rot = visii.quat(0.7071,0.7071,0,0)*visii.quat(0.7071,0,0.7071,0)
base_rot = visii.quat(1,0,0,0)

folders = glob.glob("models/*")

folder = folders[random.randint(0,len(folders))]
name  = folder.replace("models/","")
path_obj = f"{folder}/google_16k/textured.obj"
path_tex = f"{folder}/google_16k/texture_map.png"
# if "0" in name:
#     scale = 0.1 
# else:
#     scale = 0.01
scale = 0.1
print(f"loading {name}")
obj_entity = create_obj(
    name = name,
    path_obj = path_obj,
    path_tex = path_tex,
    scale = scale,
    )

bullet_id = create_physics(
    # base_position = [pos[0],pos[1],pos[2]],
    # base_orientation = [rot_random[0],rot_random[1],rot_random[2],rot_random[3]],
    base_rot = base_rot,
    aabb = [obj_entity.get_mesh().get_min_aabb_corner(), 
            obj_entity.get_mesh().get_max_aabb_corner()],
    scale = scale,
    mesh_path = path_obj,
    name = name
    )

objects_dict[name] = {        
    "visii_id": name,
    "bullet_id": bullet_id, 
    'base_rot': base_rot
}


# random location for the object
print('loaded')
from pyquaternion import Quaternion 

for key in objects_dict:
    pos_rand = [
        np.random.uniform(-2,2),
        np.random.uniform(-2,2),
        np.random.uniform(2,4),
    ]
    rq = Quaternion.random()
    rot_random = visii.quat(rq.w,rq.x,rq.y,rq.z)

    # update physics.

    p.resetBasePositionAndOrientation(
        objects_dict[key]['bullet_id'],
        pos_rand,
        [rot_random[0],rot_random[1],rot_random[2],rot_random[3]]
    )

light.get_transform().set_position(visii.vec3(pos_rand[0],pos_rand[1],pos_rand[2]) + visii.vec3(-0.5,0.5,0))
light2.get_transform().set_position(visii.vec3(pos_rand[0],pos_rand[1],pos_rand[2]) + visii.vec3(+1,-1,-1))

def render(i_frame):
    visii.render_to_png(
                width=int(opt.width), 
                height=int(opt.height), 
                samples_per_pixel=int(opt.spp),
                image_path=f"{opt.outf}/{str(i_frame).zfill(5)}.png"
                )

for i in range (500):
    p.stepSimulation()
    # if i % 20 == 0:
    if True:
        # time.sleep(1)
        # print(i)
        print(f"{opt.outf}/{str(i).zfill(5)}.png")
        for key in objects_dict:
            update_pose(objects_dict[key])

        # random_translation('camera',
        #     x_lim = [-10,10],
        #     y_lim = [-10,10],
        #     z_lim = [1,5],
        #     speed_lim = [0.02,0.05]
        #     )

        pos, rot = p.getBasePositionAndOrientation(objects_dict[key]['bullet_id'])   

        camera_entity.get_transform().look_at(
            visii.vec3(pos[0],pos[1],pos[2]),
            # visii.vec3(0,0,0),
            visii.vec3(0,0,1),
            # visii.vec3(5,0,2),
        )
        light.get_transform().look_at(
            visii.vec3(pos[0],pos[1],pos[2]),
            # visii.vec3(0,0,0),
            visii.vec3(0,0,-1),
            # visii.vec3(5,0,2),

            )
        light.get_transform().add_rotation(visii.quat(0,0,1,0))
        light2.get_transform().look_at(
            visii.vec3(pos[0],pos[1],pos[2]),
            # visii.vec3(0,0,0),
            visii.vec3(0,0,-1),
            # visii.vec3(5,0,2),

            )
        light2.get_transform().add_rotation(visii.quat(0,0,1,0))

        # camera_entity.get_camera().set_view(
        #     visii.lookAt(
        #         visii.vec3(6,0,2),
        #         visii.vec3(cubePos[0],cubePos[1],cubePos[2]),
        #         visii.vec3(0,0,1),
        #     )
        # )
        render(i)

p.disconnect()
visii.cleanup()