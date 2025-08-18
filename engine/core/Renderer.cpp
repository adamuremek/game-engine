#include "Renderer.h"
#include "World.h"
#include "MeshData.h"
#include "Material.h"

#include <core/Types.h>
#include <platform/Window.h>
#include <iostream>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <glm/glm.hpp>


#include <components/TransformComponent.h>
#include <components/ModelComponent.h>

#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

Renderer::Renderer() : m_initialized(false) {}

Renderer::~Renderer() {
    shutdown();
}


bool Renderer::init(Window* window) {
    // Get native handle and set BGFX platform data
    bgfx::PlatformData pd;
    pd.nwh = window->get_native_handle(); // THIS IS FOR WINDOWS ONLY, NEEDS TO BE CHANGED FOR OTHER OS SUPPORT
    bgfx::setPlatformData(pd);

    m_width = window->get_width();
    m_height = window->get_height();

    // Start storing init info
    bgfx::Init init;

    // Automatically select the best renderer for the current platform
    init.type = bgfx::RendererType::Direct3D12;
    init.vendorId = BGFX_PCI_ID_NONE;

    // Set the resolution and vsync
    init.resolution.width = window->get_width();
    init.resolution.height = window->get_height();
    init.resolution.reset = BGFX_RESET_VSYNC;

    // Initialize BGFX
    if (!bgfx::init(init)) {
        std::cerr << "ERROR: Failed to initialize BGFX!" << std::endl;
        return false;
    }

    // Set view 0 clear state
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    // Set view 0 to the same dimensions as the window
    bgfx::setViewRect(0, 0, 0, window->get_width(), window->get_height());
    bgfx::setViewName(0, "Render View");

    // Initialize the delta time
    m_last_dt = glfwGetTime();

    m_initialized = true;
    return true;
}

void Renderer::shutdown() {
    if (m_initialized) {
        bgfx::shutdown();
        m_initialized = false;
    }
}

float Renderer::get_delta_time() {
    double current_time = glfwGetTime();
    double delta_time = current_time - m_last_dt;
    m_last_dt = current_time;

    return static_cast<float>(delta_time);
}


void Renderer::begin_frame() {
}

void Renderer::end_frame() {
    // This is where the magic happens!
    // BGFX will perform all the draw calls submitted since beginFrame()
    bgfx::frame();
}

void Renderer::draw_frame(World &world) {
    Entity active_camera = world.get_active_camera();
    if (!active_camera) {
        std::cerr << "ERROR: No active camera set for rendering!" << std::endl;
        return;
    }

    // Set up bgfx view for the main camera
    Mat4 view_matrix = world.get_camera_view_matrix(active_camera);
    Mat4 proj_matrix = world.get_camera_proj_matrix(active_camera);
    bgfx::ViewId view_id = world.get_gamera_proj_view_id(active_camera);
    uint16_t clear_flags = world.get_camera_clear_flags(active_camera);

    bgfx::setViewClear(view_id, clear_flags, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(view_id, 0, 0, m_width, m_height);
    bgfx::setViewTransform(view_id, glm::value_ptr(view_matrix), glm::value_ptr(proj_matrix));

    // Touch the view to ensure it's cleared even if nothing is drawn
    bgfx::touch(view_id);

    // Iterate through model entities and render them
    for (auto [e, model_transform, model] : world.view<TransformComponent, ModelComponent>()) {
        Mat4 transform = model_transform.get_transform();
        bgfx::setTransform(glm::value_ptr(transform));
        bgfx::setVertexBuffer(0, model.mesh->vbh);
        bgfx::setIndexBuffer(model.mesh->ibh);
        bgfx::setState(
            BGFX_STATE_WRITE_RGB |
            BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LESS |
            BGFX_STATE_CULL_CW);
        bgfx::submit(view_id, model.material->program);
    }
}


