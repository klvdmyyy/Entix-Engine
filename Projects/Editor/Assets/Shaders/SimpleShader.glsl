#type vertex
#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec2 a_uv;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define TRANSFORMATIONS projection * view * model

void main()
{
  gl_Position = TRANSFORMATIONS * vec4(a_position, 1.0f);
  color = a_color;
}

#type fragment
#version 330 core

in vec3 color;

out vec4 outColor;

void main()
{
  outColor = vec4(color, 1.0f);
  // outColor = vec4(0.7f, 0.4f, 0.8f, 1.0f);
}
