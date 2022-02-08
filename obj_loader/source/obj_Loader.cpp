
#include "obj_loader.h"
#include "OBJProcessUtils.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	obj_Loader.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 30/09/21
// Last Edited:  01/01/21
// Brief: Parses an .obj file (and any .mtl files it refs) and returns an OBJModel object
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//declare these here as they're only needed in this cpp file.
glm::vec4 OBJGetVectorFromValue(const std::string a_strValue);
bool OBJGetKeyValuePairFromLine(const std::string& a_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue);

OBJModel* OBJLoader::OBJProcess(const std::string& a_strFilePath, const bool a_bPrintComments)
{
	if (OBJProcessUtils::GetFileType(a_strFilePath) != "obj") 
	{
		//if we aren't looking at an OBJ file return nullptr
		std::cout << "File Type Incompatible!" << std::endl;
		return nullptr;
	}
	//setup
	OBJModel* oLoadedData = new OBJModel();
	std::string line,key,value;
	OBJMesh* currentMesh = nullptr;
	std::vector<glm::vec3> vertexData, normalData;
	std::vector<glm::vec2> textureData;
	std::fstream file;
	//open file 
	file.open(a_strFilePath, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open()) 
	{
		std::cout << "Failed to open file " << a_strFilePath << std::endl;
		return nullptr;
	}
	while (!file.eof())
	{
		if (std::getline(file, line)) //read each line fo file 
		{
			if (!line.empty()) //check line has content before processing
			{
				if (OBJGetKeyValuePairFromLine(line, key, value)) 
				{
					if (key[0] == '#') //comment
					{
						if (a_bPrintComments)
						{
							std::cout << value << std::endl;
						}
					}
					else if (key == "g" || key == "o") //group/object, Blender treats them as identical so I will also
					{
						OBJGroup* g = new OBJGroup();
						g->name = value;
						if (currentMesh != nullptr)
						{
							oLoadedData->AddMesh(currentMesh);
						}
						currentMesh = new OBJMesh();
						if (oLoadedData->AddGroup(g)) std::cout << "added group" << value << std::endl;
						else std::cout << "failed to add group " << value << std::endl;
					}
					else if (key == "s") //smoothing group (not used currently)
					{
						if (OBJProcessUtils::ParseStringToInt(value))
						{
							std::cout << "smoothing group " << stoi(value) << std::endl;
						}
						else if (value == "off")
						{
							std::cout << "disabling smoothing groups" << std::endl;
						}
					}
					else if (key == "v")  //vertex data
					{
						vertexData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "vn") //normal data
					{
						normalData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "vt") //texture data
					{
						textureData.push_back(OBJGetVectorFromValue(value));
					}
					else if (key == "usemtl") //use loaded material file
					{
						OBJMaterial* m = nullptr;
						if (currentMesh == nullptr)
						{
							currentMesh = new OBJMesh();
						}

						if (oLoadedData->GetMaterial(value) == nullptr)
						{
							std::cout << "Found new MTL in .OBJ -- This is non-standard! MTL: " << value << std::endl;
						}
						else
						{
							m = oLoadedData->GetMaterial(value);
							currentMesh->m_activeMaterial = m;
						}					
						
					}
					else if (key == "mtllib") //load a new material file
					{
						OBJLoadMaterials(OBJProcessUtils::GetFileDirectory(a_strFilePath) + value,*oLoadedData,a_bPrintComments);
					}
					else if (key == "f") //face data
					{										
						if (currentMesh == nullptr)
						{
							currentMesh = new OBJMesh();
						}
						std::vector<std::string> faceComponents = OBJProcessUtils::SplitStringAtChar(value, ' ');
						unsigned int ci = currentMesh->m_verts.size();						
						for(auto iter = faceComponents.begin(); iter !=faceComponents.end(); ++iter)
						{
							OBJFace face = ProcessFace(*iter);
							OBJVertex currentVertex;
							currentVertex.SetPosition(vertexData[face.posIndex-1]);
							if (face.normIndex != 0) currentVertex.SetNormal(normalData[face.normIndex - 1]);
							if (face.uvIndex != 0) currentVertex.SetTextureCoords(textureData[face.uvIndex - 1]);
							currentMesh->m_verts.push_back(currentVertex);
						}
						bool hasNormals = !normalData.empty();
						for (unsigned int offset = 1; offset < (faceComponents.size() - 1); ++offset)
						{
							//add normal data
							currentMesh->m_indicies.push_back(ci);
							currentMesh->m_indicies.push_back(ci + offset);
							currentMesh->m_indicies.push_back(ci + offset + 1);

							if (!hasNormals)
							{
								//calculate our own normals as they were not supplied in the OBJ file
								glm::vec3 normal = currentMesh->calculateFaceNormal(ci, ci + offset, ci + offset + 1);
								currentMesh->m_verts[ci].SetNormal(normal);
								currentMesh->m_verts[ci + offset].SetNormal(normal);
								currentMesh->m_verts[ci + offset + 1].SetNormal(normal);
							}
						}					 
					}

					else
					{
						std::cout << "Unhandled Statement: " << value << std::endl;
					}
				}
			}
		}
	}
	if (currentMesh != nullptr && currentMesh != new OBJMesh()) 
	{
		oLoadedData->AddMesh(currentMesh);
	}

	//Scale Model Vertex Data
	glm::vec3 min(std::numeric_limits<float>::max());
	glm::vec3 max(std::numeric_limits<float>::min());
	//get model bounding box 
	for (unsigned int i = 0; i < oLoadedData->GetMeshCount(); ++i)
	{
		for (OBJVertex v : oLoadedData->GetMesh(i)->m_verts) 
		{
			glm::vec3 pos = v.GetPosition();
			
			max.x = std::max(max.x, pos.x);
			max.y = std::max(max.y, pos.y);
			max.z = std::max(max.z, pos.z);

			min.x = std::min(min.x, pos.x);
			min.y = std::min(min.y, pos.y);
			min.z = std::min(min.z, pos.z);
		}
	}

	float scale = 5.0f;
	for (unsigned int i = 0; i < oLoadedData->GetMeshCount(); ++i)
	{
		for (OBJVertex& v : oLoadedData->GetMesh(i)->m_verts)
		{
			v.TranslatePosition(glm::vec3(0, -min.y, 0));//align base of object with Y axis ((translate each vertex down by minY)
			v.SetPosition(v.GetPosition() *= scale/(max - min));
		}
	}

	std::cout << "File Successfully Parsed: " << a_strFilePath << std::endl;
	return oLoadedData;
}




bool OBJLoader::OBJLoadMaterials(const std::string& a_strFilePath, OBJModel& a_roLoadedData, const bool a_bPrintComments)
{
	//take in and process .mtl file, similar structure to OBJProcess, but different keys are used.
	std::fstream file;
	file.open(a_strFilePath, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << a_strFilePath << std::endl;
		return false;
	}
		OBJMaterial* currentMaterial = nullptr;
		std::string line;
		while (!file.eof())
		{
			if (std::getline(file, line))
			{
				if (!line.empty()) {
					std::string key;
					std::string value;

					if (OBJGetKeyValuePairFromLine(line, key, value))
					{
						if (key[0] == '#') //comment
						{
							if (a_bPrintComments)
							{
								std::cout << value << std::endl;
							}
						}
						else if (key == "newmtl") //new material
						{
							if (currentMaterial != nullptr)
							{
								a_roLoadedData.AddMaterial(currentMaterial);
							}

							if (a_roLoadedData.GetMaterial(value) == nullptr)
							{

								currentMaterial = new OBJMaterial();
								currentMaterial->name = value;
								std::cout << "New Material: " << value << std::endl;
							}
							else
							{
								std::cout << "Material Already Exists! " << value << std::endl;
							}
						}
						else if (key == "Ns") //specularExponent
						{
							currentMaterial->SetSpecularExponent(stof(value));
						}
						else if (key == "Ni") //refraction index / opticlal density
						{
							currentMaterial->SetDensity(stof(value));
						}
						else if (key == "D") //dissolve
						{
							currentMaterial->SetDissolve(stof(value));
						}
						else if (key == "illum") //illumination model
						{
							currentMaterial->SetIlluminationModel(stoi(value));
						}
						else if (key == "Ka") //ambience
						{
							currentMaterial->SetAmbience(OBJGetVectorFromValue(value));
						}
						else if (key == "Kd") //diffuse
						{
							currentMaterial->SetDiffuse(OBJGetVectorFromValue(value));
						}
						else if (key == "Ks") //specular highlight
						{
							currentMaterial->SetSpecular(OBJGetVectorFromValue(value));
						}
						else if (key == "Ke") //emissive
						{
							currentMaterial->SetEmissive(OBJGetVectorFromValue(value));
						}
						else if (key == "d") //opacity
						{
							currentMaterial->SetTransparency(stof(value));
						}
						else if (key == "Tr") //transparency (1-opacity)
						{
							currentMaterial->SetTransparency(1.0f - stof(value));
						}
						else if (key.substr(0,3) == "map") //texture map data
						{
							std::vector<std::string> mapData = OBJProcessUtils::SplitStringAtChar(value, ' ');
							std::string textureFileName = OBJProcessUtils::GetFileDirectory(a_strFilePath) + "Model_" + OBJProcessUtils::GetFileName(a_strFilePath) + '/' + mapData.back();
							std::string mapType = OBJProcessUtils::SplitStringAtChar(key, '_').back();

							std::cout << "Texture file at: " << textureFileName << " type: " << mapType << std::endl;

							if(currentMaterial == nullptr)
							{
								currentMaterial = new OBJMaterial();
							}

							if (mapType == "Kd") //diffuse map
							{
								currentMaterial->textureFileNames[OBJMaterial::DiffuseTexture] = textureFileName;
							}
							else if (mapType == "Ks") //specular map
							{
								currentMaterial->textureFileNames[OBJMaterial::SpecularTexture] = textureFileName;
							}
							else if (mapType == "bump") //bump/normal map
							{
								currentMaterial->textureFileNames[OBJMaterial::NormalTexture] = textureFileName;
							}

						}
						else if (key == "bump") //edge case for stupid obj standards that allow for "bump" instead of "map_bump" key??? why??? 
						{
							currentMaterial->textureFileNames[OBJMaterial::NormalTexture] = a_strFilePath + OBJProcessUtils::SplitStringAtChar(value,' ').back();
						}
						else
						{
							std::cout << "Unhandled Statement: " << value << std::endl;
						}

					}
				}
			}
			
		}

		if (currentMaterial != nullptr && currentMaterial != new OBJMaterial())
		{
			a_roLoadedData.AddMaterial(currentMaterial);
			std::cout << "New Material: " << currentMaterial->name;
		}

		std::cout << "Material File Parsed: " << a_strFilePath << std::endl;
		file.close();
		return true;
}
	


OBJFace OBJLoader::ProcessFace(std::string a_strFaceData)
{
	//contstructs an OBJ Face from input string, then return an OBJVertex containing relevant face information
	std::vector<std::string> vertIndicies = OBJProcessUtils::SplitStringAtChar(a_strFaceData, '/');
	OBJFace face = { 0,0,0 };
	face.posIndex = std::stoi(vertIndicies[0]);
	if (vertIndicies.size() > 1) {
		if (vertIndicies[1].size() > 0)
		{
			face.uvIndex = std::stoi(vertIndicies[1]);
		}
		if (vertIndicies.size() > 2)
		{
			face.normIndex = std::stoi(vertIndicies[2]);
		}
	}
	return face;
}


glm::vec4 OBJGetVectorFromValue(const std::string ac_strValue) 
{
	//parses given string into a vec4 -- this vector is implicity cast to the correct size when assigned (dangerous,scary)
	std::stringstream ss(ac_strValue);
	glm::vec4 outVertex = {0,0,0,0};
	int i = 0;
	for (std::string s; ss >> s; ++i)
	{
		((float*)(&outVertex))[i] = stof(s);
	}
	return outVertex;
}

bool OBJGetKeyValuePairFromLine(const std::string& ac_rStrLine, std::string& a_rStrOutKey, std::string& a_rStrOutValue) 
{
	//Splits a line from OBJ file into the key (or OBJStatement) and the associated value (the rest of the file)
	if (ac_rStrLine.empty()) return false;

	//get key
	size_t keyFirst = ac_rStrLine.find_first_not_of("  \t\r\n");
	if (keyFirst == std::string::npos) return false; //key was not valid
	size_t keyLast = ac_rStrLine.find_first_of("   \t\r\n", keyFirst); //start search from keyfirst
	a_rStrOutKey = ac_rStrLine.substr(keyFirst, keyLast - keyFirst);

	//get value 
	size_t valueFirst = ac_rStrLine.find_first_not_of("  \t\r\n", keyLast); //start search from where we left off 
	if (valueFirst == std::string::npos) return false; //value was not valid
	size_t valueLast = ac_rStrLine.find_last_not_of("  \t\r\n") + 1; //add one to get last character as method used is exclusive
	a_rStrOutValue = ac_rStrLine.substr(valueFirst, valueLast - valueFirst);

	return true;
}
