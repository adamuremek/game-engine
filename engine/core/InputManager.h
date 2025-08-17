#ifndef GAME_INPUTMANAGER_H
#define GAME_INPUTMANAGER_H

#include "Types.h"
#include <platform/Window.h>
#include <map>
#include <string>
#include <vector>
#include <memory>

class InputAction {
public:
    std::string name;
    std::vector<int> bindings;
};

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    void init(std::shared_ptr<Window> window);
    void update();
    void shutdown();

    void add_action(const std::string& name, KeyCode default_key);
    void add_action(const std::string& name, MouseButton default_btn);

    ActionState get_action_state(const std::string& name);

    bool is_action_pressed(const std::string& name);
    bool is_action_held(const std::string& name);
    bool is_action_released(const std::string& name);

    Vec2 get_mouse_position();
    Vec2 get_mouse_delta();

private:
    static KeyCode translate_glfw_key(int glfw_key);
    static MouseButton translate_glfw_mouse_button(int glfw_mouse_button);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    void handle_key(int key, int action);
    void handle_mouse_button(int button, int action);
    void handle_cursor_position(double xpos, double ypos);

    std::shared_ptr<Window> m_window;

    std::map<int, int> m_current_key_states;
    std::map<int, int> m_previous_key_states;

    std::map<std::string, InputAction> m_actions;

    Vec2 m_current_mouse_pos;
    Vec2 m_previous_mouse_pos;
};

#endif //GAME_INPUTMANAGER_H