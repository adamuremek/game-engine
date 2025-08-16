#include "App.h"


#include<iostream>

App::App(const std::shared_ptr<World> &world)
    : m_world(world)
{}


void App::init() {
    std::cout << "Initializing App..." << std::endl;

    // Create the window
    m_window = std::make_unique<Window>();
    if (!m_window->init("BGFX Engine", 1920, 1080, false)) {
        std::cerr << "Failed to create window." << std::endl;
        return;
    }

    // Create renderer
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->init(m_window.get())) {
        // Handle initialization error
        std::cerr << "Failed to initialize renderer" << std::endl;
        return;
    }
    std::cout << "App Initialized!" << std::endl;

    m_initialized = true;
}

void App::tick() {
    //Initialize the app
    if (!m_initialized) {
        std::cerr << "App is not initialized. Cannot start up." << std::endl;
        return;
    }

    m_window->poll_events();

    // Execute buffered mutate commands
    m_world->execute_commands();

    // Draw the frame
    m_renderer->begin_frame(); // Begin the frame
    m_renderer->draw_frame(*m_world);
    m_renderer->end_frame(); // End the frame
}

bool App::should_close() {
    return m_window->should_close();
}


void App::shutdown() {
    // Shutdown subsystems in reverse order of initialization
    std::cout << "Shutting Down App..." << std::endl;
    if (m_window) {
        m_window->shutdown();
    }
}


