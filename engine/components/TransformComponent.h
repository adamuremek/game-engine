#ifndef GAME_TRANSFORMCOMPONENT_H
#define GAME_TRANSFORMCOMPONENT_H

#include <interfaces/IComponent.h>
#include <core/Types.h>

class TransformComponent : public IComponent {
public:
    TransformComponent();
    TransformComponent(const Vec3& position);

    // Rotations
    void rotate(const Quat& rotation);
    void rotate(const Vec3& axis, float angle_rads);

    void look_at(const Vec3& target, const Vec3& up);

    Vec3 forward();
    Vec3 right();
    Vec3 up();

    // Getters
    Vec3 get_position();
    Vec3 get_local_position();
    Quat get_rotation();
    Quat get_local_rotation();
    Vec3 get_scale();
    Vec3 get_local_scale();
    Mat4 get_transform();

    // Setters
    void set_position(const Vec3& position);
    void set_local_position(const Vec3& position);
    void set_rotation(const Quat& rotation);
    void set_local_rotation(const Quat& rotation);
    void set_scale(const Vec3& scale);
    void set_local_scale(const Vec3& scale);

private:
    Mat4 m_transform;
    Mat4 m_local_transform;
};

#endif //GAME_TRANSFORMCOMPONENT_H