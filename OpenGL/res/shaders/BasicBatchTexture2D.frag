#version 330 core

layout(location = 0) out vec4 o_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[2];
//uniform sampler2D u_Texture;
//uniform sampler2D u_AltTexture;

void main()
{
	int index = int(v_TexIndex);
	o_Color = texture(u_Textures[index], v_TexCoord);
//	vec4 texColor = texture(u_Texture, v_TexCoord);
//	vec4 texColorAlt = texture(u_AltTexture, v_TexCoord);
//
//	if (v_TexIndex == 0)
//		o_Color = texColor;
//	else
//		o_Color = texColorAlt;
}
