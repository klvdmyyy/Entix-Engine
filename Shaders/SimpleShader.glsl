#type vertex
#version 330 core

layout (location = 0) in vec3 a_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

#define TRANSFORMATIONS projection * view * model

void main()
{
  gl_Position = TRANSFORMATIONS * vec4(a_position, 1.0f);
}

#type fragment
#version 330 core

out vec4 color;

void main()
{
  color = vec4(0.7f, 0.4f, 0.8f, 1.0f);
}
