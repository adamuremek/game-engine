$input v_worldPos

#include <bgfx_shader.sh>


void main()
{
    // Use the X and Z coordinates from the varying
    vec2 pos = v_worldPos.xz;
    
    // Anti-Aliased Grid Lines
    vec2 gridWidth = fwidth(pos) * 1.0; 
    vec2 gridLines = abs(fract(pos - gridWidth * 0.5) - 0.5);
    vec2 grid = gridLines / gridWidth;
    float lineValue = min(grid.x, grid.y);
    float gridFactor = 1.0 - min(lineValue, 1.0);
    
    // Color definitions
    vec4 gridColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 axisColor = vec4(0.8, 0.2, 0.2, 1.0); // Red X-axis
    vec4 axisColorZ = vec4(0.2, 0.2, 0.8, 1.0); // Blue Z-axis

    // Highlight the Axes
    float axisWidth = 2.0;
    float onZAxis = 1.0 - min(abs(pos.x) / (gridWidth.x * axisWidth), 1.0);
    float onXAxis = 1.0 - min(abs(pos.y) / (gridWidth.y * axisWidth), 1.0);
    
    // Mix the grid and axis colors
    vec4 finalColor = mix(gridColor, axisColor, onZAxis);
    finalColor = mix(finalColor, axisColorZ, onXAxis);
    
    // The final color for the fragment
    gl_FragColor = vec4(finalColor.rgb, finalColor.a * gridFactor);
    
    // Discard transparent pixels
    if (gl_FragColor.a < 0.01)
    {
        discard;
    }
}