#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoord;

out vec3 vertPos;
out vec3 vertNormal;
out vec2 vertUV;

uniform mat4 ProjectionViewMatrix;
vec4 pos4;

const int maxBoids = 250;

uniform mat4 matrixList[maxBoids];

void main()
{
    pos4 = vec4(position.xyz,1.0f);
	vertUV = uvCoord;
	vertNormal = normal;
	mat4 InstanceMatrix = matrixList[gl_InstanceID];
	vertPos = (InstanceMatrix * pos4).xyz;
	gl_Position = (ProjectionViewMatrix * InstanceMatrix) * pos4;
}
