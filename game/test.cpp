#include <Engine.h>
#include <iostream>

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
}

void test(World& world, float dt) {
    std::cout << dt << std::endl;
}

SYSTEMS_ON_STARTUP(start_test);
SYSTEMS_ON_UPDATE(test);