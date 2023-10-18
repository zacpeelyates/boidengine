// File: Texture.h
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/01/24
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class: Texture declarations

#ifndef TEXTURE_H
#define TEXTURE_H
#include <string>

class Texture 
{
public:
	Texture() {};
	~Texture();

	bool Load(const std::string& a_inFilename);
	bool LoadCubemap(const std::string& a_inFileDirectory);
	const std::string& GetFileName() {return m_filename; }
	unsigned int GetTextureID() const {return m_textureID; }
	void GetDimensions(unsigned int& a_uiWidth, unsigned int& a_uiHeight) const;

private:
	std::string m_filename;
	unsigned int m_width; 
	unsigned int m_height;
	unsigned int m_textureID;
};

#endif // TEXTURE_H
