#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in float a_TexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	gl_Position = u_MVP * a_Position;
}
