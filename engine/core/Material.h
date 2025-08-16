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
        if (bgfx::isValid(u_base_color)) {
            bgfx::destroy(u_base_color);
        }
        if (bgfx::isValid(u_tint)) {
            bgfx::destroy(u_tint);
        }
        if (bgfx::isValid(s_base_color)) {
            bgfx::destroy(s_base_color);
        }
    }

    // Compiled shader handles (bgfx .bin loaded via createShader)
    bgfx::ShaderHandle vsh{BGFX_INVALID_HANDLE};
    bgfx::ShaderHandle fsh{BGFX_INVALID_HANDLE};
    // Linked program
    bgfx::ProgramHandle program{BGFX_INVALID_HANDLE};

    // Common fixed pipeline state for this material
    uint64_t state =
        BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
        BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

    // Shared uniforms/samplers used by the shaders (optional, but handy)
    bgfx::UniformHandle u_base_color{BGFX_INVALID_HANDLE}; // float4
    bgfx::UniformHandle u_tint{BGFX_INVALID_HANDLE}; // float4
    bgfx::UniformHandle s_base_color{BGFX_INVALID_HANDLE}; // sampler2D
};

#endif //GAME_MATERIAL_H