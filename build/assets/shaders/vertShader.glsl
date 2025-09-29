#version 430
layout (location=0) in vec3 position;

void main(void)
{ 
    // Simple vertex shader - just pass through position
    gl_Position = vec4(position, 1.0);
}
