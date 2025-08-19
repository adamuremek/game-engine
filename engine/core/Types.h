#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <functional>
#include <string>

using Vec3 = glm::vec3;
using Vec2 = glm::vec2;
using Vec4 = glm::vec4;
using Quat = glm::quat;
using Mat4 = glm::mat4;

using Entity = uint32_t;

enum class ProjectionType {
    Perspective,
    Orthographic
};

enum class ComponentType {
    Transform,
    Camera,
    Model
};

enum class ActionState {
    Idle,
    Pressed,
    Held,
    Released
};

enum class InputType {
    Key,
    MouseButton
};

enum class KeyCode {
    // Letters
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

    // Numbers
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

    // Special Keys
    Space,
    Enter,
    Escape,
    Tab,
    LeftShift,
    LeftControl,
    LeftAlt,
    RightShift,
    RightControl,
    RightAlt,

    // Arrow Keys
    Up,
    Down,
    Left,
    Right,

    // Unspecified key
    Unknown
};

// A custom, library-agnostic enum for mouse buttons.
enum class MouseButton {
    Left,
    Right,
    Middle,
    Unknown
};

enum class CursorMode {
    Normal,
    Hidden,
    Disabled
};


class InputBinding {
public:
    InputType type;
    int code;
};

class CompoundBinding {
public:
    std::vector<InputBinding> bindings;
    bool requires_mouse_move = false;
};

class InputAction {
public:
    std::string name;
    std::vector<CompoundBinding> compounds;

    std::function<void()> on_press;
    std::function<void()> on_release;
    std::function<void()> on_hold;
    std::function<void(Vec2)> on_continuous;
};

#endif //GAME_TYPES_H