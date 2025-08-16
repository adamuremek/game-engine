$input a_position
$output v_worldPos

#include <bgfx_shader.sh>

void main()
{
    const float gridScale = 1000.0;
    vec3 scaled_position = a_position * gridScale;

    // Calculate the world position using the NEW scaled position
    vec4 worldPos = mul(u_model[0], vec4(scaled_position, 1.0));
    
    // Pass world position to the fragment shader
    v_worldPos = worldPos.xyz;
    
    // Calculate the final screen position
    gl_Position = mul(u_viewProj, worldPos);
}