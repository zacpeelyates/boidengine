
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	Texture.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 05/01/22
// Last Edited:  09/01/22
// Brief: Texture class function definitions for loading single textures & cubemaps.
// Also contains useful info about texture in member variable
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

class Texture 
{
public:
	Texture() {};
	~Texture();

	bool Load(std::string a_inFilename);
	bool LoadCubemap(std::string a_inFileDirectory);
	const std::string& GetFileName() {return m_filename; }
	unsigned int GetTextureID() {return m_textureID; }
	void GetDimensions(unsigned int& a_uiWidth, unsigned int& a_uiHeight) const;

private:
	std::string m_filename;
	unsigned int m_width; 
	unsigned int m_height;
	unsigned int m_textureID;
};

#endif // TEXTURE_H
