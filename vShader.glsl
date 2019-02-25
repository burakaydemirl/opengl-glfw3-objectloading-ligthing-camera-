#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 scale;
uniform mat4 rotate;
uniform mat4 translate;

void main()
{
    vs_out.FragPos = vec3(translate * rotate * scale * vec4(aPos, 1.0f));
    vs_out.Normal = aNormal;
    gl_Position = projection * view * translate * rotate * scale * vec4(aPos, 1.0);
}
