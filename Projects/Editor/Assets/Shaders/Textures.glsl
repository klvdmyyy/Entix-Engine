#type vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 3) in vec2 a_uv;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define TRANSFORMATIONS projection * view * model

void main()
{
    gl_Position = TRANSFORMATIONS * vec4(a_position, 1.0f);
    color = a_color;
    texCoord = a_uv;
}

#type fragment
#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D imageTexture;

void main()
{
    outColor = vec4(color, 1.0);
    // outColor = texture(imageTexture, texCoord);
}