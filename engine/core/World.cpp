#include "World.h"
#include <iostream>

World::World()
    :   m_active_camera(0),
        m_command_queue(std::make_unique<CommandQueue>()),
        m_entity_pool(std::make_unique<EntityPool>(MAX_ENTITIES)),
        m_entity_sparse_set(std::make_unique<EntitySparseSet>(MAX_ENTITIES)),
        m_input_manager(std::make_unique<InputManager>()),
        m_mesh_manager(std::make_unique<MeshManager>()),
        m_material_manager(std::make_unique<MaterialManager>()),

        m_transform_component_pool(std::make_unique<ComponentPool<TransformComponent>>()),
        m_camera_component_pool(std::make_unique<ComponentPool<CameraComponent>>()),
        m_model_component_pool(std::make_unique<ComponentPool<ModelComponent>>())
{}





Entity World::get_active_camera() const {
    return m_active_camera;
}

Mat4 World::get_camera_view_matrix(Entity camera) {
    CameraComponent* camera_comp = m_camera_component_pool->get(camera);
    TransformComponent* transform_comp = m_transform_component_pool->get(camera);

    return !camera_comp || !transform_comp ? Mat4(1.0f) :
    camera_comp->get_view_matrix(
        transform_comp->get_position(),
        transform_comp->forward(),
        transform_comp->up());
}


Mat4 World::get_camera_proj_matrix(Entity camera) {
    CameraComponent* camera_comp = m_camera_component_pool->get(camera);

    return !camera_comp ? Mat4(1.0f) : camera_comp->get_projection_matrix();
}

bgfx::ViewId World::get_gamera_proj_view_id(Entity camera) {
    CameraComponent* camera_comp = m_camera_component_pool->get(camera);

    return !camera_comp ? 0 : camera_comp->view_id;
}

uint16_t World::get_camera_clear_flags(Entity camera) {
    CameraComponent* camera_comp = m_camera_component_pool->get(camera);

    return !camera_comp ? 0 : camera_comp->clear_flags;
}

void World::load_mesh(Entity entity, const std::string &file_path) {
    m_command_queue->submit([this, entity, file_path] {
        ModelComponent* model_comp = m_model_component_pool->get(entity);

        if (!model_comp) {
            std::cerr << "Entity does not contain a model component." << std::endl;
            return;
        }
        model_comp->mesh = m_mesh_manager->load(file_path);
    });
}

void World::load_material(Entity entity, const std::string &material_id) {
    m_command_queue->submit([this, entity, material_id] {
        ModelComponent* model_comp = m_model_component_pool->get(entity);

        if (!model_comp) {
            std::cerr << "Entity does not contain a model component." << std::endl;
            return;
        }

        model_comp->material = m_material_manager->load(material_id);
    });
}




void World::execute_commands() {
    for (auto cmd : m_command_queue->drain()) {
        cmd();
    }
}




Entity World::create_entity() {
    Entity entity = m_entity_pool->create();
    m_command_queue->submit([this, entity] {
        m_entity_sparse_set->insert(entity);
    });

    return entity;
}

void World::set_active_camera(Entity entity) {
    m_command_queue->submit([this, entity] {
        m_active_camera = entity;
    });
}

void World::set_input_manager_window(const std::shared_ptr<Window> &window) {
    m_input_manager->set_window(window);
}

void World::set_cursor_mode(CursorMode mode) {
    m_input_manager->set_cursor_mode(mode);
}


// ====================== Transform Interface ==================== //

Vec3 World::get_forward(Entity entity) {
    TransformComponent* transform_comp = m_transform_component_pool->get(entity);

    if (!transform_comp) {
        std::cerr << "Entity does not contain a transform component." << std::endl;
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    return transform_comp->forward();
}

Vec3 World::get_right(Entity entity) {
    TransformComponent* transform_comp = m_transform_component_pool->get(entity);

    if (!transform_comp) {
        std::cerr << "Entity does not contain a transform component." << std::endl;
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    return transform_comp->right();
}

Vec3 World::get_up(Entity entity) {
    TransformComponent* transform_comp = m_transform_component_pool->get(entity);

    if (!transform_comp) {
        std::cerr << "Entity does not contain a transform component." << std::endl;
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    return transform_comp->up();
}

Vec3 World::get_position(Entity entity) {
    TransformComponent* transform_comp = m_transform_component_pool->get(entity);

    if (!transform_comp) {
        std::cerr << "Entity does not contain a transform component." << std::endl;
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    return transform_comp->get_position();
}

void World::set_position(Entity entity, Vec3 pos) {
    m_command_queue->submit([this, entity, pos] {
        TransformComponent* transform_comp = m_transform_component_pool->get(entity);

        if (!transform_comp) {
            std::cerr << "Entity does not contain a transform component." << std::endl;
            return;
        }

        transform_comp->set_position(pos);
    });
}

void World::set_rotation(Entity entity, Vec3 angle_rot) {
    m_command_queue->submit([this, entity, angle_rot] {
        TransformComponent* transform_comp = m_transform_component_pool->get(entity);

        if (!transform_comp) {
            std::cerr << "Entity does not contain a transform component." << std::endl;
            return;
        }

        Quat rot(glm::radians(angle_rot));

        transform_comp->set_rotation(rot);
    });
}

void World::set_rotation(Entity entity, Quat rot) {
    m_command_queue->submit([this, entity, rot] {
        TransformComponent* transform_comp = m_transform_component_pool->get(entity);

        if (!transform_comp) {
            std::cerr << "Entity does not contain a transform component." << std::endl;
            return;
        }

        transform_comp->set_rotation(rot);
    });
}

void World::rotate(Entity entity, Vec3 axis, float angle_rad) {
    m_command_queue->submit([this, entity, axis, angle_rad] {
        TransformComponent* transform_comp = m_transform_component_pool->get(entity);

        if (!transform_comp) {
            std::cerr << "Entity does not contain a transform component." << std::endl;
            return;
        }

        transform_comp->rotate(axis, angle_rad);
    });
}
// =============================================================== //



void World::add_component(Entity entity, ComponentType component_type) {
    switch (component_type) {
        case ComponentType::Transform:
            m_command_queue->submit([this, entity] {
                m_transform_component_pool->add(entity, std::make_unique<TransformComponent>());
            });
            break;
        case ComponentType::Camera:
            m_command_queue->submit([this, entity] {
                m_camera_component_pool->add(entity, std::make_unique<CameraComponent>());
            });
            break;
        case ComponentType::Model:
            m_command_queue->submit([this, entity] {
                m_model_component_pool->add(entity, std::make_unique<ModelComponent>());
            });
            break;
    }
}

// =================== Input Manager Interface =================== //
void World::query_inputs() {
    m_input_manager->query_inputs();
}

void World::add_input_action(const std::string &name, KeyCode key) {
    m_input_manager->add_action(name, key);
}

void World::add_input_action(const std::string &name, MouseButton btn) {
    m_input_manager->add_action(name, btn);
}

bool World::input_action_pressd(const std::string &name) {
    return m_input_manager->is_action_pressed(name);
}

bool World::input_action_held(const std::string &name) {
    return m_input_manager->is_action_held(name);
}

bool World::input_action_released(const std::string &name) {
    return m_input_manager->is_action_released(name);
}

Vec2 World::get_mouse_delta() {
    return m_input_manager->get_mouse_delta();
}
// =============================================================== //




