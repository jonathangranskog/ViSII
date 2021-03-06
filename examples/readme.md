# ViSii examples

This folder contains different examples showing different capabilities for visii. 
The first example uses interactive mode, whereas the rest of the examples 
use headless mode and save local images (look for `tmp.png` in the `examples/` 
folder). 
We also provide baseline outputs for what visii should be producing. 
You can download them through `sh download_results.sh`. 

There is also a python `requirements.txt` for different packages needed to run all the examples. 
Some examples have minimal requirements, e.g., `visii` only whereas others use packages that 
makes the final task easier. 

Please note that if you have more than one GPU, you might run into problems if they 
are not the same. Please use only one GPU with the following command `CUDA_VISIBLE_DEVICES=0`. 
For example you could run: 
```
CUDA_VISIBLE_DEVICES=0 python 1.simple_scene.py
```


## 0.helloworld.py
This is simple script that opens up a window with gaussian noise displayed. 


## 1.simple_scene.py
This shows a sphere on top of a mirror floor. You should see how to set up a simple scene 
with visii, the light is provided by the dome. 


## 2.random_scene.py
This shows how to generate a randomized scene with different meshes and how to randomize 
the materials of the object. This is similar to how domain randomization look like. 


## 3.pybullet.py
This example creates a video in a folder, you are going to need `ffmpeg` to generate 
the final video, which is called `output.mp4` and it is located in the `examples/` folder.


## 4.load_obj_file.py
This script assumes you have downloaded the content for the demos using `sh download_content.sh`. 
You can also use your own obj file, see the arguments to the script. 


## 5.lights.py
This script presents how to add lights to your scenes. Lights in visii are meshes, 
so we show how to do different lights in this example. You can also check how the different 
materials react to each other, e.g., the cube is transmissive (it is transparent). 


## 6.textures.py
This script assumes you have downloaded the content for the demos using `sh download_content.sh`. 
Moreover the textures can be used to set specific material propreties, such as 
roughness (see `7.procedural_texture.py`). 


## 7.procedural_texture.py
This script shows how to use procedurally loading a texture and applying it to a roughness 
material property. The final picture looks like a mirror with spots on them. 


## 8.obj_scene_loader.py
This scene uses downloaded content, so make sure to run `sh download_content.sh` to get 
the obj scene. This scene is a bathroom showing how you can change specific material or objects
from a large obj scene.   


## 9.meta_data_exporting.py
This is a very simple scene showing different export that visii does naturally, e.g., normals. 


## 10.light_texture.py
This is the same scene to `5.lights.py` but we added textures to the lights. Check out 
how the color light bounces on the objects and the floor. 


## Notes
All these examples were developed and tested on Ubuntu 18.04 with cuda 10.2, NVIDIA drivers
440.100 and using a NVIDIA RTX 2080 ti. 


## TODO
- exporting data is missing segmentation id for objects