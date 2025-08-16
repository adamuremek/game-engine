#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Vec4 = glm::vec4;
using Quat = glm::quat;
using Mat4 = glm::mat4;

using Entity = uint32_t;

enum class ProjectionType {
    Perspective,
    Orthographic
};

enum class ComponentType {
    Transform,
    Camera,
    Model
};

#endif //GAME_TYPES_H