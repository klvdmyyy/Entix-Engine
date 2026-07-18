#type vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 3) in vec2 a_uv;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define TRANSFORMATIONS projection * view * model

void main()
{
    gl_Position = TRANSFORMATIONS * vec4(a_position, 1.0f);
    texCoord = a_uv;
}

#type fragment
#version 330 core

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D imageTexture;

void main()
{
    outColor = texture(imageTexture, texCoord);
}