#include "World.h"
#include <iostream>

World::World()
    :   m_active_camera(0),
        m_command_queue(std::make_unique<CommandQueue>()),
        m_entity_pool(std::make_unique<EntityPool>(MAX_ENTITIES)),
        m_entity_sparse_set(std::make_unique<EntitySparseSet>(MAX_ENTITIES)),
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



