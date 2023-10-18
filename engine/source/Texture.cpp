// File: Texture.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/01/24
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Texture class function implementations for loading single textures & cubemaps

#include "Texture.h"

#include <iostream>
#include <ostream>
#include <glad/glad.h>
#include <stb_image.h>

Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

auto Texture::Load(const std::string& a_inFilename) -> bool
{
	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_set_flip_vertically_on_load(1); //flip non-cubemap textures 

	unsigned char* imageData = stbi_load(a_inFilename.c_str(), &width, &height, &channels, 4);
	//4 expected channels (RGBA)
	if (imageData != nullptr)
	{
		//store file info in this texture object
		m_filename = a_inFilename;
		m_width = width;
		m_height = height;
		//generate opengl texture data
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);


		stbi_image_free(imageData);
		std::cout << "Loaded Image: " << a_inFilename << std::endl;
		return true;
	}

	return false;
}

auto Texture::LoadCubemap(const std::string& a_inFileDirectory) -> bool
{
	m_filename = a_inFileDirectory;
	m_width = 0;
	m_height = 0;

	const std::string faces[6]{"right", "left", "top", "bottom", "back", "front"}; //GL_TEXTURE_CUBEMAP expected order
	const std::string fileType = ".jpg"; //testing, should be var to support multiple types
	int width = 0;
	int height = 0;
	int channels = 0;
	stbi_set_flip_vertically_on_load(0); //do not flip cubemap textures
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	for (int i = 0; i < 6; ++i)
	{
		std::string filepath = a_inFileDirectory + "/" + faces[i] + fileType;
		//combine passed in directory with expected filenames
		unsigned char* imageData = stbi_load(filepath.c_str(), &width, &height, &channels, 3);
		if (imageData == nullptr)
		{
			return false; //early out if texture not found
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
		             imageData); //GL_TEXTURE_CUBE_MAP + i hits memory addresses in order shown in faces vector
		//total width and height of the cubemap stored
		m_width += width;
		m_height += height;
		stbi_image_free(imageData);
	}
	//generate cubemap params once all parts of texture are loaded
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	std::cout << "Loaded Cubemap at: " << a_inFileDirectory << std::endl;
	return true;
}


void Texture::GetDimensions(unsigned int& a_uiWidth, unsigned int& a_uiHeight) const
{
	//return texture info 
	a_uiWidth = m_width;
	a_uiHeight = m_height;
}
