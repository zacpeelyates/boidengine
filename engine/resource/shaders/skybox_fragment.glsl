#version 400

out vec4 outputColor;
in vec3 UVCoords;
uniform samplerCube skybox;


void main()
{
	outputColor = texture(skybox,UVCoords);
}

