#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

#include <bgfx/bgfx.h>

struct Vertex {
    float x, y, z; // Position
    float nx, ny, nz; // Normal
    float u, v; // UV

    static const bgfx::VertexLayout& vertex_layout() {
        static const bgfx::VertexLayout s_layout = [] {
            bgfx::VertexLayout layout;
            layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
            return layout;
        }();

        return s_layout;
    }
};

// Make sure the size of the struct is correct for a float
static_assert(sizeof(Vertex) == 8 * sizeof(float), "Unexpected padding in Vertex");

#endif //GAME_VERTEX_H