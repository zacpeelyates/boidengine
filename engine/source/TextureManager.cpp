#include "TextureManager.h"
#include "Texture.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:	TextureManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 05/01/22
// Last Edited:  09/01/22
// Brief: Function implementations for singleton class that loads (loading implementation in Texture::Load()) and stores
// texture IDs and a count of how many times they are referenced.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//singleton implementation
TextureManager* TextureManager::m_instance = nullptr;
TextureManager* TextureManager::CreateInstance()
{
	if (m_instance == nullptr) 
	{
		m_instance = new TextureManager();
	}
	return m_instance;
}

TextureManager* TextureManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = CreateInstance();
	}
	return m_instance;
}

void TextureManager::DestroyInstance()
{
	if (m_instance != nullptr) 
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
//function implementations
inline bool TextureManager::TextureExists(const char* a_pName)
{
	return m_TextureMap.find(a_pName) != m_TextureMap.end();
}

unsigned int TextureManager::LoadTexture(const char* a_pfilename, bool bIsCubemap)
{
	if (a_pfilename != nullptr)
	{
		auto dictionaryIter = m_TextureMap.find(a_pfilename);
		if (dictionaryIter != m_TextureMap.end()) 
		{
			//existing texture, increment reference count
			TextureRef& texRef = (TextureRef&)(dictionaryIter->second);
			++texRef.refCount;
			return texRef.pTexture->GetTextureID();
		}
		else 
		{
			//load in new texture
			Texture* pTexture = new Texture();
			if (bIsCubemap ? pTexture->LoadCubemap(a_pfilename) : pTexture->Load(a_pfilename)) //call correct loading function for texture type
			{
				//store texture in map
				TextureRef texRef = { pTexture, 1 };
				m_TextureMap[a_pfilename] = texRef;
				return pTexture->GetTextureID(); //return loaded id
			}
			else 
			{
				//failed to load
				std::cout << "Failed to Load Texture! " << a_pfilename << std::endl;
				delete pTexture;
				return 0;
			}
		}
	}
	return 0;
}

unsigned int TextureManager::GetTexture(const char* a_pfilename)
{
	auto dictionaryIter = m_TextureMap.find(a_pfilename); //not using TextureExists here are we want to reference the iterator later
	if (dictionaryIter != m_TextureMap.end()) 
	{
		TextureRef& texRef = (TextureRef&)dictionaryIter->second;
		texRef.refCount++;
		return texRef.pTexture->GetTextureID(); //return found texture ID
	}
	return -1; //could not get texture
}

void TextureManager::ReleaseTexture(unsigned int a_texture)
{
	for (auto dictionaryIter = m_TextureMap.begin(); dictionaryIter != m_TextureMap.end(); ++dictionaryIter)
	{
		TextureRef currentRef = (TextureRef&)dictionaryIter->second;
		if (a_texture == currentRef.pTexture->GetTextureID())
		{
			if (--currentRef.refCount == 0) //decrease reference count
			{
				//if there are no remaining references, destroy stored texture
				delete currentRef.pTexture;
				currentRef.pTexture = nullptr;
				m_TextureMap.erase(dictionaryIter);
				break;
			}
		}
	}
}

TextureManager::~TextureManager()
{
	m_TextureMap.clear();
}





