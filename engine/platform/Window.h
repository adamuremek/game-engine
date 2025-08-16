#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <cstdint>

struct GLFWwindow;

class Window {
public:
    Window();
    ~Window();

    bool init(const char* title, uint32_t width, uint32_t height, bool opengl_context);
    void shutdown();

    bool should_close();
    void poll_events();

    // Getters
    void* get_native_handle();
    GLFWwindow* get_glfw_window() const { return m_handle; }
    int32_t get_width();
    int32_t get_height();
    int32_t get_mouse_x();
    int32_t get_mouse_y();
    uint8_t get_mouse_buttons();
    int32_t get_mouse_scroll();
    int32_t get_last_char();

private:
    GLFWwindow* m_handle;
    static bool m_glfw_initialized;

    int32_t m_width;
    int32_t m_height;
    int32_t m_mouse_x;
    int32_t m_mouse_y;
    uint8_t m_mouse_buttons;
    int32_t m_mouse_scroll;
    int32_t m_last_char;

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void char_callback(GLFWwindow* window, unsigned int codepoint);
};

#endif //GAME_WINDOW_H