#include <iostream>
#include <thread>
#include <memory>

#include <entry/App.h>
#include <entry/Editor.h>
#include <core/World.h>
#include <core/Types.h>

int main() {
    auto world = std::make_shared<World>();

    // Create a main viewport camera
    Entity cam = world->create_entity();
    world->add_component(cam, ComponentType::Transform);
    world->add_component(cam, ComponentType::Camera);
    world->set_active_camera(cam);
    world->set_position(cam, Vec3(0.0f, 5.0f, 20.0f));

    // Create a coordinate plane
    Entity cp = world->create_entity();
    world->add_component(cp, ComponentType::Transform);
    world->add_component(cp, ComponentType::Model);
    world->set_rotation(cp, Vec3(-90.0f, 0.0f, 0.0f));
    world->load_mesh(cp, "C:/Users/aruem/Desktop/Game/models/Plane.fbx");
    world->load_material(cp, "coordinate_plane");


    // Create a testing cube
    Entity cube = world->create_entity();
    world->add_component(cube, ComponentType::Transform);
    world->add_component(cube, ComponentType::Model);
    world->load_mesh(cube, "C:/Users/aruem/Desktop/Game/models/Cube.fbx");
    world->load_material(cube, "color");


    Editor editor;
    App app(world);

    editor.init();
    app.init();

    while (!editor.should_close() && !app.should_close()) {
        editor.tick();
        app.tick();
    }

    return 0;
}