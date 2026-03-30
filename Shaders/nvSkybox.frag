#version 330 core

out vec4 FragColor;

in vec3 TexCoord;

uniform samplerCube skybox;

void main()
{
	vec4 baseColor = texture(skybox, TexCoord);
	FragColor = vec4(0.f, baseColor.y, 0.f, 1.0f);
}