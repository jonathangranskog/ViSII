import visii
import argparse

parser = argparse.ArgumentParser()

parser.add_argument('--spp', 
                    default=100,
                    type=int,
                    help = "number of sample per pixel, higher the more costly")
parser.add_argument('--width', 
                    default=1280,
                    type=int,
                    help = 'image output width')
parser.add_argument('--height', 
                    default=720,
                    type=int,
                    help = 'image output height')
parser.add_argument('--noise',
                    action='store_true',
                    default=False,
                    help = "if added the output of the ray tracing is not sent to optix's denoiser")
parser.add_argument('--path_obj',
                    default='content/dragon/dragon.obj',
                    help = "path to the obj mesh you want to load")
parser.add_argument('--out',
                    default='tmp.png',
                    help = "output filename")

opt = parser.parse_args()

# # # # # # # # # # # # # # # # # # # # # # # # #
visii.initialize_headless()

if not opt.noise is True: 
    visii.enable_denoiser()

camera = visii.entity.create(
    name = "camera",
    transform = visii.transform.create("camera"),
    camera = visii.camera.create_perspective_from_fov(
        name = "camera", 
        field_of_view = 0.785398, 
        aspect = float(opt.width)/float(opt.height)
    )
)

camera.get_transform().look_at(
    visii.vec3(0,0.1,0.1), # look at (world coordinate)
    visii.vec3(0,0,1), # up vector
    visii.vec3(0,3.0,0.2), # camera_origin    
)
visii.set_camera_entity(camera)

visii.set_dome_light_intensity(1)

# # # # # # # # # # # # # # # # # # # # # # # # #

floor = visii.entity.create(
    name = "floor",
    mesh = visii.mesh.create_plane("floor", size = visii.vec2(10,10)),
    material = visii.material.create("floor", base_color = visii.vec3(.5, .5, .5), roughness = 0.0, metallic = 1.0),
    transform = visii.transform.create("floor", position = visii.vec3(0,0,-.3))
)

# Next, let's load an obj
mesh = visii.mesh.create_from_obj("obj", opt.path_obj)

# Now, lets make three instances of that mesh
obj1 = visii.entity.create(
    name="obj1",
    mesh = mesh,
    transform = visii.transform.create("obj1"),
    material = visii.material.create("obj1")
)

obj2 = visii.entity.create(
    name="obj2",
    mesh = mesh,
    transform = visii.transform.create("obj2"),
    material = visii.material.create("obj2")
)

obj3 = visii.entity.create(
    name="obj3",
    mesh = mesh,
    transform = visii.transform.create("obj3"),
    material = visii.material.create("obj3")
)

obj4 = visii.entity.create(
    name="obj4",
    mesh = mesh,
    transform = visii.transform.create("obj4"),
    material = visii.material.create("obj4")
)

# place those objects into the scene

# lets set the obj_entity up
obj1.get_transform().set_position( visii.vec3(-1.5, 0, 0))
obj1.get_transform().set_rotation( visii.quat(0.7071, 0.7071, 0, 0))
obj1.get_material().set_base_color(visii.vec3(1,0,0))  
obj1.get_material().set_roughness(0.7)   
obj1.get_material().set_specular(1)   
obj1.get_material().set_sheen(1)

obj2.get_transform().set_position( visii.vec3(-.5, 0, 0))
obj2.get_transform().set_rotation( visii.quat(0.7071, 0.7071, 0, 0))
obj2.get_material().set_base_color(visii.vec3(0,1,0))  
obj2.get_material().set_roughness(0.7)   
obj2.get_material().set_specular(1)   
obj2.get_material().set_sheen(1)

obj3.get_transform().set_position( visii.vec3(.5, 0, 0))
obj3.get_transform().set_rotation( visii.quat(0.7071, 0.7071, 0, 0))
obj3.get_material().set_base_color(visii.vec3(0,0,1))  
obj3.get_material().set_roughness(0.7)   
obj3.get_material().set_specular(1)   
obj3.get_material().set_sheen(1)

obj4.get_transform().set_position( visii.vec3(1.5, 0, 0))
obj4.get_transform().set_rotation( visii.quat(0.7071, 0.7071, 0, 0))
obj4.get_material().set_base_color(visii.vec3(.5,.5,.5))  
obj4.get_material().set_roughness(0.7)   
obj4.get_material().set_specular(1)   
obj4.get_material().set_sheen(1)


# Use linear motion blur on the first object...
obj1.get_transform().set_linear_velocity(visii.vec3(.0, .0, .2))

# angular motion blur on the second object...
obj2.get_transform().set_angular_velocity(visii.quat(1, visii.pi() / 16, visii.pi() / 16, visii.pi() / 16))

# and scalar motion blur on the third object
obj3.get_transform().set_scalar_velocity(visii.vec3(-.5, -.5, -.5))

# # # # # # # # # # # # # # # # # # # # # # # # #

visii.render_to_png(
    width=int(opt.width), 
    height=int(opt.height), 
    samples_per_pixel=int(opt.spp),
    image_path=f"{opt.out}"
)

# let's clean up the GPU
visii.cleanup()