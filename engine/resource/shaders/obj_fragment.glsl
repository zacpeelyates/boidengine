#version 400
smooth in vec3 vertPos;
smooth in vec3 vertNormal;
smooth in vec2 vertUV;

out vec4 outputColor;

uniform vec3 camPos;
uniform vec3 kA;
uniform vec3 kD;
uniform vec3 kS;
uniform vec3 lightPos;
uniform vec4 lightColor;

uniform bool useNormal;
uniform bool useSpecular;
uniform bool useDiffuse;

uniform float nS;

vec3 iA = vec3(0.25f);
vec3 iD = vec3(1.0f);
vec3 iS = vec3(1.0f);



//texture
uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;


void main()
{
	float gamma = 2.2;
	vec3 lightDir = normalize(vec3(0.0f) - lightPos);
	//texture
	vec3 normalData = pow(useNormal ? texture(NormalTexture,vertUV).rgb : vec3(1),vec3(1.0/gamma));
	vec3 specularData =  pow(useSpecular ? texture(SpecularTexture,vertUV).rgb : vec3(1),vec3(1.0/gamma));
	vec3 diffuseData = pow(useDiffuse ? texture(DiffuseTexture,vertUV).rgb : vec3(1),vec3(1.0/gamma));
	//lighting 
	vec3 modifiedNormal = vertNormal * normalData;
	vec3 Ambient = kA * iA;
	float nDl = max(0.0f,dot(normalize(modifiedNormal),-lightDir));
	vec3 Diffuse = kD * iD * nDl * diffuseData;

	vec3 R = reflect(lightDir, normalize(modifiedNormal));
	vec3 E = normalize(camPos - vertPos);

	float SpecTerm = pow(max(0.0f,dot(R,E)),nS);
	vec3 Specular = kS * iS * SpecTerm * specularData;
	outputColor = vec4((Ambient + Diffuse + Specular), 1.0f) + lightColor;
}

