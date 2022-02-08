#version 400

layout (location = 0) in vec3 position;

out vec3 UVCoords;

uniform mat4 ProjectionViewMatrix;

void main()
{
	UVCoords = vec3(position.xy,-position.z); //convert from cubemap cood system to opengl coord system (left-hand/right-hand)
	gl_Position = (ProjectionViewMatrix * vec4(UVCoords,1.0)).xyww;
	
}
