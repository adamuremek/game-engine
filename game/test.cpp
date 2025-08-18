#include <Engine.h>
#include <iomanip>
#include <iostream>

#include <glm/glm.hpp>

void start_test(World& world) {
    // Create a main viewport camera
    Entity cam = world.create_entity();
    world.add_component(cam, ComponentType::Transform);
    world.add_component(cam, ComponentType::Camera);
    world.set_active_camera(cam);
    world.set_position(cam, Vec3(0.0f, 5.0f, 20.0f));

    // Create a coordinate plane
    Entity cp = world.create_entity();
    world.add_component(cp, ComponentType::Transform);
    world.add_component(cp, ComponentType::Model);
    world.set_rotation(cp, Vec3(-90.0f, 0.0f, 0.0f));
    world.load_mesh(cp, "C:/Users/aruem/Desktop/game-engine/models/Plane.fbx");
    world.load_material(cp, "coordinate_plane");


    // Create a testing cube
    Entity cube = world.create_entity();
    world.add_component(cube, ComponentType::Transform);
    world.add_component(cube, ComponentType::Model);
    world.load_mesh(cube, "C:/Users/aruem/Desktop/game-engine/models/Cube.fbx");
    world.load_material(cube, "color");

    world.add_input_action("PanCamera", MouseButton::Right);
    world.add_input_action("CameraMoveForward", KeyCode::W);
    world.add_input_action("CameraMoveBackward", KeyCode::S);
    world.add_input_action("CameraMoveLeft", KeyCode::A);
    world.add_input_action("CameraMoveRight", KeyCode::D);
    world.add_input_action("CameraMoveUp", KeyCode::E);
    world.add_input_action("CameraMoveDown", KeyCode::Q);
    world.add_input_action("SpeedUpCamera", KeyCode::LeftShift);
}

struct State {
    float yaw = 0.0f;
    float pitch = 0.0f;
};

void test(World& world, float dt, State& data) {
    Entity cam = world.get_active_camera();

    if (world.input_action_held("PanCamera")) {
        Vec2 delta = world.get_mouse_delta();
        // std::cout << std::fixed << std::setprecision(3) << "(x=" << delta.x << ", y=" << delta.y << ")" << std::endl;

        data.yaw -= delta.x * 0.01f;
        data.pitch -= delta.y * 0.01f;

        Quat q_yaw = glm::angleAxis(data.yaw, Vec3(0.0f, 1.0f, 0.0f));
        Quat q_pitch = glm::angleAxis(data.pitch, Vec3(1.0f, 0.0f, 0.0f));
        Quat orient = q_yaw * q_pitch;

        world.set_rotation(cam, orient);
    }

    Vec3 translate = Vec3(0.0f);
    Vec3 forward = world.get_forward(cam);
    Vec3 right = world.get_right(cam);
    Vec3 up = world.get_up(cam);
    float cam_speed = 7.0f;

    if (world.input_action_held("CameraMoveForward")) {
        translate += forward;
    }

    if (world.input_action_held("CameraMoveBackward")) {
        translate -= forward;
    }

    if (world.input_action_held("CameraMoveLeft")) {
        translate -= right;
    }

    if (world.input_action_held("CameraMoveRight")) {
        translate += right;
    }

    if (world.input_action_held("CameraMoveUp")) {
        translate += up;
    }

    if (world.input_action_held("CameraMoveDown")) {
        translate -= up;
    }

    if (world.input_action_held("SpeedUpCamera")) {
        cam_speed = 14.0f;
    }


    Vec3 cur_pos = world.get_position(cam);
    world.set_position(cam, cur_pos + translate * dt * cam_speed);
}

SYSTEMS_ON_STARTUP(start_test);
SYSTEMS_ON_UPDATE(test, State);