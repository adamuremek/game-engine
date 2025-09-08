#include "CameraComponent.hpp"
#include <glm/glm.hpp>

Mat4 CameraComponent::get_view_matrix(Vec3 position, Vec3 forward, Vec3 up) {
    Vec3 target = position + forward;
    return glm::lookAt(position, target, up);
}

Mat4 CameraComponent::get_projection_matrix() {
    if (projection_type == ProjectionType::Perspective) {
        return glm::perspective(
            glm::radians(this->fov_degrees),
            this->aspect_ratio,
            this->near_clip,
            this->far_clip);
    } else {
        float half_height = this->ortho_size * 0.5f;
        float half_width = half_height * this->aspect_ratio;
        return glm::ortho(
            -half_width, half_width,
            -half_height, half_height,
            this->near_clip,
            this->far_clip);
    }
}


