#ifndef GAME_MATERIAL_H
#define GAME_MATERIAL_H

#include <bgfx/bgfx.h>
#include <vector>

class Material {
public:
    Material() = default;
    ~Material() {
        if (bgfx::isValid(program)) {
            bgfx::destroy(program);
        }
        if (bgfx::isValid(vsh)) {
            bgfx::destroy(vsh);
        }
        if (bgfx::isValid(fsh)) {
            bgfx::destroy(fsh);
        }
    }

    // Compiled shader handles (bgfx .bin loaded via createShader)
    bgfx::ShaderHandle vsh{BGFX_INVALID_HANDLE};
    bgfx::ShaderHandle fsh{BGFX_INVALID_HANDLE};
    // Linked program
    bgfx::ProgramHandle program{BGFX_INVALID_HANDLE};

    // Common fixed pipeline state for this material
    uint64_t state =
            BGFX_STATE_WRITE_RGB |
            BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z |
            BGFX_STATE_DEPTH_TEST_LESS |
            BGFX_STATE_CULL_CW;


    void set_backface_culling(bool enabled) {
        if (enabled) {
            state |= BGFX_STATE_CULL_CW;
        } else {
            state &= ~BGFX_STATE_CULL_CW;
        }
    }
};

#endif //GAME_MATERIAL_H