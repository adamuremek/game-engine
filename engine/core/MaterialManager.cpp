#include "MaterialManager.h"
#include "Material.h"
#include <bgfx/bgfx.h>
#include <memory>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

std::shared_ptr<Material> MaterialManager::load(const std::string &material_id) {
    auto it  = m_material_cache.find(material_id);
    if (it != m_material_cache.end()) {
        if (auto shared = it->second.lock()) {
            return shared;
        } else {
            m_material_cache.erase(it); // clean up expired weak pointers
        }
    }

    auto material = load_from_id(material_id);
    if (material) {
        m_material_cache[material_id] = material;
    }

    return material;
}


bgfx::ShaderHandle MaterialManager::load_shader_bin(const std::string &file_path) {
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open shader: " << file_path << std::endl;
        return BGFX_INVALID_HANDLE;
    }

    const std::streamsize file_size = file.tellg();
    if (file_size <= 0) {
        std::cerr << "Shader is empty or unreadable: " << file_path << std::endl;
        return BGFX_INVALID_HANDLE;
    }
    file.seekg(0, std::ios::beg);

    // Allocate +1 byte for null terminator
    std::vector<uint8_t> buffer(static_cast<size_t>(file_size) + 1u);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), file_size)) {
        std::cerr << "Failed to read shader: " << file_path << std::endl;
        return BGFX_INVALID_HANDLE;
    }
    buffer[static_cast<size_t>(file_size)] = '\0';

    const bgfx::Memory* mem = bgfx::copy(buffer.data(), static_cast<uint32_t>(buffer.size()));
    return bgfx::createShader(mem);
}

std::shared_ptr<Material> MaterialManager::load_from_id(const std::string &material_id) {
    fs::path shader_dir = "C:/Users/aruem/Desktop/game-engine/shaders";
    fs::path mat_dir = shader_dir / material_id;

    if (!fs::exists(mat_dir) || !fs::is_directory(mat_dir)) {
        std::cerr << "Material directory not found: " << mat_dir << std::endl;
        return nullptr;
    }

    fs::path vs_path = mat_dir / ("vs_" + material_id + ".bin");
    fs::path fs_path = mat_dir / ("fs_" + material_id + ".bin");

    if (!fs::exists(vs_path) || !fs::exists(fs_path)) {
        std::cerr << "Missing shader file(s) for material: " << material_id << std::endl;
        return nullptr;
    }

    auto vsh = load_shader_bin(vs_path.string());
    auto fsh = load_shader_bin(fs_path.string());
    if (!bgfx::isValid(vsh) || !bgfx::isValid(fsh)) {
        if (bgfx::isValid(vsh)) bgfx::destroy(vsh);
        if (bgfx::isValid(fsh)) bgfx::destroy(fsh);
        std::cerr << "Material shaders are invalid for: " << material_id << std::endl;
        return nullptr;
    }

    auto prog = bgfx::createProgram(vsh, fsh, false);
    if (!bgfx::isValid(prog)) {
        bgfx::destroy(vsh);
        bgfx::destroy(fsh);
        std::cerr << "Failed to create program for: " << material_id << std::endl;
        return nullptr;
    }

    auto mat = std::make_shared<Material>();
    mat->vsh = vsh;
    mat->fsh = fsh;
    mat->program = prog;


    return mat;
}


