////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////

///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/resource/ResourceManager.h>
/////////////////////////////////////////////////////////////

namespace nero
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{

	}


	void ResourceManager::loadDirectory(const std::string& directory)
	{
		//addDirectory(directory, true);

		//loadDirectory();
	}


	FontHolder::Ptr ResourceManager::getFontHolder()
	{
		return m_FontHolder;
	}

	SoundHolder::Ptr ResourceManager::getSoundHolder()
	{
		return m_SoundHolder;
	}

	MusicHolder::Ptr ResourceManager::getMusicHolder()
	{
		return m_MusicHolder;
	}

	ShaderHolder::Ptr ResourceManager::getShaderHolder()
	{
		return m_ShaderHolder;
	}

	ScriptHolder::Ptr ResourceManager::getLuaScriptHolder()
	{
		return m_LuaScriptHolder;
	}

	TextureHolder::Ptr ResourceManager::getTextureHolder()
	{
		return m_TextureHolder;
	}

	AnimationHolder::Ptr ResourceManager::getAnimationHolder()
	{
		return m_AnimationHolder;
	}

}
