#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_MVP;

out vec4 v_Color;

void main()
{
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	v_Color = a_Color;
	gl_Position = u_MVP * a_Position;
}
