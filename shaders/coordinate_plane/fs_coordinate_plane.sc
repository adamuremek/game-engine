$input v_worldPos

#include <bgfx_shader.sh>


void main()
{
    vec2 pos = v_worldPos.xz;

    // --- 1. FIND ALL GRID LINES (Corrected Method) ---
    vec2 gridWidth = fwidth(pos) * 1.0;

    // This is a more robust way to calculate the distance to the nearest integer line.
    // It creates a triangle wave that is 0.0 on the line and 1.0 mid-cell.
    vec2 gridDist = 2.0 * min(fract(pos), 1.0 - fract(pos));

    // Anti-alias the line by dividing the distance by the pixel width.
    vec2 gridLines = gridDist / gridWidth;
    
    // We take the minimum distance to either a horizontal or vertical line.
    float lineValue = min(gridLines.x, gridLines.y);
    
    // Invert the value so it's 1.0 on the line and 0.0 elsewhere.
    float gridFactor = 1.0 - min(lineValue, 1.0);

    // If the pixel is not part of a line, discard it.
    if (gridFactor < 0.01)
    {
        discard;
    }

    // --- 2. DETERMINE THE CORRECT COLOR ---
    vec4 finalColor;

    vec4 gridColor     = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 xAxisPosColor = vec4(1.0, 0.1, 0.1, 1.0); // Bright pure red
    vec4 xAxisNegColor = vec4(0.4, 0.0, 0.0, 1.0); // Very dark red
    vec4 zAxisPosColor = vec4(0.1, 0.1, 1.0, 1.0); // Bright pure blue
    vec4 zAxisNegColor = vec4(0.0, 0.0, 0.4, 1.0); // Very dark blue

    // Use the precise check to see if this line is an axis.
    if (abs(pos.x) < gridWidth.x) // Z-axis
    {
        finalColor = mix(zAxisNegColor, zAxisPosColor, step(0.0, pos.y));
    }
    else if (abs(pos.y) < gridWidth.y) // X-axis
    {
        finalColor = mix(xAxisNegColor, xAxisPosColor, step(0.0, pos.x));
    }
    else // Regular grid line
    {
        finalColor = gridColor;
    }

    // --- 3. SET THE FINAL FRAGMENT COLOR ---
    gl_FragColor = vec4(finalColor.rgb, gridFactor);
}