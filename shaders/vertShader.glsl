#version 430
layout (location=0) in vec3 position;

uniform mat4 v_matrix;  // View matrix
uniform mat4 p_matrix;  // Projection matrix

out vec4 varyingColor;

void main(void)
{ 
    // Simply use the matrices provided by the C++ code
    gl_Position = p_matrix * v_matrix * vec4(position, 1.0);

    // Calculate a simple color based on position
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}