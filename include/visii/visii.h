#pragma once
#include <visii/entity.h>
#include <visii/transform.h>
#include <visii/material.h>
#include <visii/mesh.h>
#include <visii/camera.h>
#include <visii/light.h>
#include <visii/texture.h>

/**
  * Initializes various backend systems required to render scene data.
  * 
  * @param window_on_top Keeps the window opened during an interactive session on top of any other windows.
*/
void initializeInteractive(bool window_on_top = false);

/**
  * Initializes various backend systems required to render scene data.
  * 
  * This call avoids using any OpenGL resources, to enable 
*/
void initializeHeadless();

/**
  * Cleans up any allocated resources, closes windows and shuts down any running backend systems.
*/
void cleanup();

/** 
 * Tells the renderer which camera entity to use for rendering. The transform 
 * component of this camera entity places the camera into the world, and the
 * camera component of this camera entity describes the perspective to use, the 
 * field of view, the depth of field, and other "analog" camera properties.
 * 
 * @param camera_entity The entity containing a camera and transform component, to use for rendering. 
 */
void setCameraEntity(Entity* camera_entity);


/** 
 * Sets the intensity, or brightness, that the dome light (aka environment light) will emit it's color.
 * 
 * @param intensity How powerful the dome light is in emitting light
 */ 
void setDomeLightIntensity(float intensity);

/** 
 * Sets the texture used to color the dome light (aka the environment). 
 * Textures are sampled using a 2D to 3D latitude/longitude strategy.
 * 
 * @param texture The texture to sample for the dome light.
 */ 
void setDomeLightTexture(Texture* texture);

/** 
 * Sets the rotation to apply to the dome light (aka the environment). 
 * 
 * @param rotation The rotation to apply to the dome light
 */ 
void setDomeLightRotation(glm::quat rotation);

/** 
 * Clamps the indirect light intensity during progressive image refinement. 
 * This reduces fireflies from indirect lighting, but also removes energy, and biases the resulting image.
 * 
 * @param clamp The maximum intensity that indirect lighting can contribute per frame. A value of 0 disables indirect light clamping.
 */ 
void setIndirectLightingClamp(float clamp);

/** 
 * Clamps the direct light intensity during progressive image refinement. 
 * This reduces fireflies from direct lighting, but also removes energy, and biases the resulting image.
 * 
 * @param clamp The maximum intensity that direct lighting can contribute per frame. A value of 0 disables direct light clamping.
 */ 
void setDirectLightingClamp(float clamp);

/** 
 * Sets the maximum number of times that a ray originating from the camera can bounce through the scene to accumulate light.
 * For scenes containing only rough surfaces, this max bounce depth can be set to lower values.
 * For scenes containing complex transmissive or reflective objects like glass or metals, this 
 * max bounce depth might need to be increased to accurately render these objects. 
 * 
 * @param depth The maximum number of bounces allowed per ray.
 */ 
void setMaxBounceDepth(uint32_t depth);

/**
  * If using interactive mode, resizes the window to the specified dimensions.
  * 
  * @param width The width to resize the window to
  * @param height The height to resize the window to
*/
void resizeWindow(uint32_t width, uint32_t height);

/** Enables the Optix denoiser. */
void enableDenoiser();

/** Disables the Optix denoiser. */
void disableDenoiser();

/** 
 * Renders the current scene, returning the resulting framebuffer back to the user directly.
 * 
 * @param width The width of the image to render
 * @param height The height of the image to render
 * @param samples_per_pixel The number of rays to trace and accumulate per pixel.
*/
std::vector<float> render(uint32_t width, uint32_t height, uint32_t samples_per_pixel);

/** 
 * Renders the current scene, saving the resulting framebuffer to an HDR image on disk.
 * 
 * @param width The width of the image to render
 * @param height The height of the image to render
 * @param samples_per_pixel The number of rays to trace and accumulate per pixel.
 * @param image_path The path to use to save the HDR file, including the extension.
*/
void renderToHDR(uint32_t width, uint32_t height, uint32_t samples_per_pixel, std::string image_path);

/** 
 * Renders the current scene, saving the resulting framebuffer to a PNG image on disk.
 * 
 * @param width The width of the image to render
 * @param height The height of the image to render
 * @param samples_per_pixel The number of rays to trace and accumulate per pixel.
 * @param image_path The path to use to save the PNG file, including the extension.
*/
void renderToPNG(uint32_t width, uint32_t height, uint32_t samples_per_pixel, std::string image_path);

/** 
 * Renders out metadata used to render the current scene, returning the resulting framebuffer back to the user directly.
 * 
 * @param width The width of the image to render
 * @param height The height of the image to render
 * @param start_frame The start seed to feed into the random number generator
 * @param frame_count The number of frames to accumulate the resulting framebuffers by. For ID data, this should be set to 0.
 * @param bounce The number of bounces required to reach the vertex whose metadata result should come from. A value of 0
 * would save data for objects directly visible to the camera, a value of 1 would save reflections/refractions, etc.
 * @param options Indicates the data to return. Current possible values include 
 * "none" for rendering out raw path traced data, "depth" to render the distance between the previous path vertex to the current one,
 * "position" for rendering out the world space position of the path vertex, "normal" for rendering out the world space normal of the 
 * path vertex, "entity_id" for rendering out the entity ID whose surface the path vertex hit, "denoise_normal" for rendering out
 * the normal buffer supplied to the Optix denoiser, and "denoise_albedo" for rendering out the albedo supplied to the Optix denoiser.   
*/
std::vector<float> renderData(uint32_t width, uint32_t height, uint32_t start_frame, uint32_t frame_count, uint32_t bounce, std::string options);

/**
 * Imports an OBJ containing scene data. 
 * First, any materials described by the mtl file are used to generate Material components.
 * Next, any textures required by those materials will be loaded. 
 * After that, all shapes will be separated by material.
 * For each separated shape, an entity is created to attach a transform, mesh, and material component together.
 * These shapes are then translated so that the transform component is centered at the centroid of the shape.
 * Finally, any specified position, scale, and/or rotation are applied to the generated transforms.
 * 
 * @param name_prefix A string used to uniquely prefix any generated component names by.
 * @param filepath The path for the OBJ file to load
 * @param mtl_base_dir The path to the directory containing the corresponding .mtl file for the OBJ being loaded
 * @param position A change in position to apply to all entities generated by this function
 * @param position A change in scale to apply to all entities generated by this function
 * @param position A change in rotation to apply to all entities generated by this function
*/
std::vector<Entity*> importOBJ(std::string name_prefix, std::string file_path, std::string mtl_base_dir, 
        glm::vec3 position = glm::vec3(0.0f), 
        glm::vec3 scale = glm::vec3(1.0f),
        glm::quat rotation = glm::angleAxis(0.0f, glm::vec3(1.0f, 0.0f, 0.0f)));