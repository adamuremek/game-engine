#ifndef GAME_RIGIDBODYCOMPONENT_HPP
#define GAME_RIGIDBODYCOMPONENT_HPP

#include <core/Types.h>
#include <interfaces/IComponent.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

class RigidBodyComponent : public IComponent {
public:
    BodyType type = BodyType::Static;
    float mass = 1.0f;
    float friction = 0.2f;
    float restitution = 0.0f; // Bounciness
    bool allow_sleeping = true;

    JPH::BodyID body_id;
    bool initialized = false;
};

#endif //GAME_RIGIDBODYCOMPONENT_HPP