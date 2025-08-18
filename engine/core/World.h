#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <memory>
#include <bgfx/bgfx.h>
#include <string>

#include "ComponentPool.h"
#include "CommandQueue.h"
#include "EntitySparseSet.h"
#include "Types.h"
#include "View2.h"
#include "EntityPool.h"
#include "InputManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"

#include <platform/Window.h>

#include <components/TransformComponent.h>
#include <components/ModelComponent.h>
#include <components/CameraComponent.h>

inline size_t MAX_ENTITIES = 10000;

class World {
public:
    World();

    // GETTERS
    Entity get_active_camera() const;

    Mat4 get_camera_view_matrix(Entity camera);
    Mat4 get_camera_proj_matrix(Entity camera);
    bgfx::ViewId get_gamera_proj_view_id(Entity camera);
    uint16_t get_camera_clear_flags(Entity camera);



    void load_mesh(Entity entity, const std::string& file_path);
    void load_material(Entity entity, const std::string& material_id);

    template<typename A, typename B>
    View2<A,B> view() {
        return View2<A, B>(*pool<A>(), *pool<B>());
    }

    void execute_commands();

    // CREATION
    Entity create_entity();

    // ADDERS
    void add_component(Entity entity, ComponentType component_type);

    // =================== Input Manager Interface =================== //
    void query_inputs();
    void add_input_action(const std::string& name, KeyCode key);
    void add_input_action(const std::string& name, MouseButton btn);
    bool input_action_pressd(const std::string& name);
    bool input_action_held(const std::string& name);
    Vec2 get_mouse_delta();
    // =============================================================== //


    // SETTERS
    void set_active_camera(Entity entity);

    void set_input_manager_window(const std::shared_ptr<Window>& window);

    // ====================== Transform Interface ==================== //
    Vec3 get_forward(Entity entity);
    Vec3 get_right(Entity entity);
    Vec3 get_up(Entity entity);
    Vec3 get_position(Entity entity);

    void set_position(Entity entity, Vec3 pos);
    void set_rotation(Entity entity, Vec3 angle_rot);
    void set_rotation(Entity entity, Quat rot);
    void rotate(Entity entity, Vec3 axis, float angle_rad);
    void set_scale();
    // =============================================================== //


private:
    Entity m_active_camera;
    std::unique_ptr<CommandQueue> m_command_queue;
    std::unique_ptr<EntityPool> m_entity_pool;
    std::unique_ptr<EntitySparseSet> m_entity_sparse_set;

    std::unique_ptr<InputManager> m_input_manager;
    std::unique_ptr<MeshManager> m_mesh_manager;
    std::unique_ptr<MaterialManager> m_material_manager;


    std::unique_ptr<ComponentPool<TransformComponent>> m_transform_component_pool;
    std::unique_ptr<ComponentPool<CameraComponent>> m_camera_component_pool;
    std::unique_ptr<ComponentPool<ModelComponent>> m_model_component_pool;

    template<typename C> ComponentPool<C>* pool();
};

template<> inline ComponentPool<TransformComponent>* World::pool<TransformComponent>() {
    return m_transform_component_pool.get();
}
template<> inline ComponentPool<CameraComponent>* World::pool<CameraComponent>() {
    return m_camera_component_pool.get();
}
template<> inline ComponentPool<ModelComponent>* World::pool<ModelComponent>() {
    return m_model_component_pool.get();
}

#endif //GAME_WORLD_H