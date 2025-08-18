#include "TransformComponent.h"

TransformComponent::TransformComponent() {
    set_position(Vec3(0.0f, 0.0f, 0.0f));
    set_rotation(Quat(1.0f, 0.0f, 0.0f, 0.0f));
    set_scale(Vec3(1.0f, 1.0f, 1.0f));
}

TransformComponent::TransformComponent(const Vec3 &position) {
    set_local_position(position);
    set_local_rotation(Quat(1.0f, 0.0f, 0.0f, 0.0f));
    set_local_scale(Vec3(1.0f, 1.0f, 1.0f));
}

void TransformComponent::rotate(const Quat &rotation) {
    set_rotation(glm::normalize(get_rotation() * rotation));
}

void TransformComponent::rotate(const Vec3 &axis, float angle_rads) {
    rotate(glm::angleAxis(angle_rads, glm::normalize(axis)));
}

void TransformComponent::look_at(const Vec3 &target, const Vec3 &up) {
    Mat4 look = glm::lookAt(get_local_position(), target, up);
    set_local_rotation(glm::quat_cast(glm::inverse(look)));
}

Vec3 TransformComponent::forward() {
    return glm::normalize(get_rotation() * Vec3(0, 0, -1));
}

Vec3 TransformComponent::right() {
    return glm::normalize(get_rotation() * Vec3(1, 0, 0));
}

Vec3 TransformComponent::up() {
    return glm::normalize(get_rotation() * Vec3(0, 1, 0));
}

Vec3 TransformComponent::get_position() {
    return Vec3(m_transform[3]);
}

Vec3 TransformComponent::get_local_position() {
    return Vec3(m_local_transform[3]);
}

Quat TransformComponent::get_rotation() {
    return glm::quat_cast(m_transform);
}

Quat TransformComponent::get_local_rotation() {
    return glm::quat_cast(m_local_transform);
}

Vec3 TransformComponent::get_scale() {
    return Vec3(
        glm::length(Vec3(m_transform[0])),
        glm::length(Vec3(m_transform[1])),
        glm::length(Vec3(m_transform[2]))
        );
}

Vec3 TransformComponent::get_local_scale() {
    return Vec3(
        glm::length(Vec3(m_local_transform[0])),
        glm::length(Vec3(m_local_transform[1])),
        glm::length(Vec3(m_local_transform[2]))
        );
}

Mat4 TransformComponent::get_transform() {
    return m_transform;
}


void TransformComponent::set_position(const Vec3 &position) {
    m_transform[3] = Vec4(position, 1.0f);
}

void TransformComponent::set_local_position(const Vec3 &position) {
    m_local_transform[3] = Vec4(position, 1.0f);
}

void TransformComponent::set_rotation(const Quat &rotation) {
    Vec3 scale = get_scale();
    Vec3 pos = get_position();
    m_transform = glm::mat4_cast(rotation);
    m_transform = glm::scale(m_transform, scale);
    set_position(pos);
}

void TransformComponent::set_local_rotation(const Quat &rotation) {
    Vec3 scale = get_local_scale();
    Vec3 pos = get_local_position();
    m_local_transform = glm::mat4_cast(rotation);
    m_local_transform = glm::scale(m_local_transform, scale);
    set_local_position(pos);
}

void TransformComponent::set_scale(const Vec3 &scale) {
    Quat rot = get_rotation();
    Vec3 pos = get_position();
    m_transform = glm::mat4_cast(rot);
    m_transform = glm::scale(m_transform, scale);
    set_position(pos);
}

void TransformComponent::set_local_scale(const Vec3 &scale) {
    Quat rot = get_local_rotation();
    Vec3 pos = get_local_position();
    m_local_transform = glm::mat4_cast(rot);
    m_local_transform = glm::scale(m_local_transform, scale);
    set_local_position(pos);
}


















