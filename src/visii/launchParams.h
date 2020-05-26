#pragma once

#include <owl/owl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

#include <visii/entity_struct.h>
#include <visii/transform_struct.h>
#include <visii/material_struct.h>
#include <visii/camera_struct.h>
#include <visii/mesh_struct.h>

struct LaunchParams {
    glm::ivec2 frameSize;
    glm::vec4 *fbPtr;
    uint32_t *accumPtr;
    OptixTraversableHandle world;

    EntityStruct    cameraEntity;
    EntityStruct    *entities = nullptr;
    TransformStruct *transforms = nullptr;
    MaterialStruct  *materials = nullptr;
    CameraStruct    *cameras = nullptr;
    MeshStruct      *meshes = nullptr;
    uint32_t        *instanceToEntityMap = nullptr;
};
