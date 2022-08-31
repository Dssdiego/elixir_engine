#version 450

// inputs from code/pipeline
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_uv;

// send to fragment shader
layout(location = 0) out vec2 uv;

// push constants
layout(push_constant) uniform Push {
    mat4 transform;
    vec3 color;
} push;

// shader code
void main() {
    gl_Position = push.transform * vec4(v_position, 1.0f);
}
