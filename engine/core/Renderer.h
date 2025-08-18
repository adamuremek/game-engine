#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <cstdint>

class Window;
class World;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init(Window* window);
    void shutdown();

    float get_delta_time();
    void begin_frame();
    void end_frame();
    void draw_frame(World& world);

private:
    bool m_initialized = false;
    int32_t m_width = 0;
    int32_t m_height = 0;
    double m_last_dt = 0.0;
};

#endif //GAME_RENDERER_H