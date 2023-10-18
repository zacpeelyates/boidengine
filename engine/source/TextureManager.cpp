// File: TextureManager.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 2022/01/24
// Date Edited: 2022/05/26
// ct5037boidengine
// 
// Description of class:  Function implementations for singleton class that loads (loading implementation in Texture::Load()) and stores
// texture IDs and a count of how many times they are referenced.

#include "TextureManager.h"

#include <iostream>

#include "Texture.h"

//singleton implementation
TextureManager* TextureManager::m_instance = nullptr;

auto TextureManager::CreateInstance() -> TextureManager*
{
	if (m_instance == nullptr)
	{
		m_instance = new TextureManager();
	}
	return m_instance;
}

auto TextureManager::GetInstance() -> TextureManager*
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
inline auto TextureManager::TextureExists(const char* a_pName) -> bool
{
	return m_TextureMap.find(a_pName) != m_TextureMap.end();
}

auto TextureManager::LoadTexture(const char* a_pfilename, bool bIsCubemap) -> unsigned int
{
	if (a_pfilename != nullptr)
	{
		const auto dictionaryIter = m_TextureMap.find(a_pfilename);
		if (dictionaryIter != m_TextureMap.end())
		{
			//existing texture, increment reference count
			auto& texRef = dictionaryIter->second;
			++texRef.refCount;
			return texRef.pTexture->GetTextureID();
		}
		//load in new texture
		auto* const pTexture = new Texture();
		if (bIsCubemap ? pTexture->LoadCubemap(a_pfilename) : pTexture->Load(a_pfilename))
		//call correct loading function for texture type
		{
			//store texture in map
			const TextureRef texRef = {pTexture, 1};
			m_TextureMap[a_pfilename] = texRef;
			return pTexture->GetTextureID(); //return loaded id
		}
		//failed to load
		std::cout << "Failed to Load Texture! " << a_pfilename << std::endl;
		delete pTexture;
		return 0;
	}
	return 0;
}

auto TextureManager::GetTexture(const char* a_pfilename) -> unsigned int
{
	const auto dictionaryIter = m_TextureMap.find(a_pfilename);
	//not using TextureExists here are we want to reference the iterator later
	if (dictionaryIter != m_TextureMap.end())
	{
		auto& texRef = dictionaryIter->second;
		texRef.refCount++;
		return texRef.pTexture->GetTextureID(); //return found texture ID
	}
	return -1; //could not get texture
}

void TextureManager::ReleaseTexture(unsigned int a_texture)
{
	for (auto dictionaryIter = m_TextureMap.begin(); dictionaryIter != m_TextureMap.end(); ++dictionaryIter)
	{
		auto currentRef = static_cast<TextureRef&>(dictionaryIter->second);
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
