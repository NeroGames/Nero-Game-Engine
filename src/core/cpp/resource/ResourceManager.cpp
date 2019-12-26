////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////

///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/utility/FileUtil.h>
/////////////////////////////////////////////////////////////

namespace nero
{
	ResourceManager::ResourceManager():
		m_Setting()
	{
		m_TextureHolder		= std::make_shared<TextureHolder>();
		m_AnimationHolder	= std::make_shared<AnimationHolder>();
		m_FontHolder		= std::make_shared<FontHolder>();
		m_SoundHolder		= std::make_shared<SoundHolder>();
		m_MusicHolder		= std::make_shared<MusicHolder>();
		m_LightmapHolder	= std::make_shared<TextureHolder>();

		//m_LuaScriptHolder	= std::make_shared<ScriptHolder>();
		//m_ShaderHolder		= std::make_shared<ShaderHolder>();
	}

	ResourceManager::ResourceManager(const Setting& setting):
		m_Setting(setting)
	{
		m_TextureHolder		= std::make_shared<TextureHolder>(setting.getSetting("texture"));
		m_AnimationHolder	= std::make_shared<AnimationHolder>(setting.getSetting("animation"));
		m_FontHolder		= std::make_shared<FontHolder>(setting.getSetting("font"));
		m_SoundHolder		= std::make_shared<SoundHolder>(setting.getSetting("sound"));
		m_MusicHolder		= std::make_shared<MusicHolder>(setting.getSetting("music"));
		m_LightmapHolder	= std::make_shared<TextureHolder>(setting.getSetting("texture"));

		//m_LuaScriptHolder	= std::make_shared<ScriptHolder>(setting.getSetting("script"));
		//m_ShaderHolder		= std::make_shared<ShaderHolder>(setting.getSetting("shader"));
	}

	ResourceManager::~ResourceManager()
	{
		destroy();
	}

	void ResourceManager::destroy()
	{

	}

	void ResourceManager::setSetting(const Setting& setting)
	{
		m_Setting = setting;

		m_TextureHolder->setSetting(setting.getSetting("texture"));
		m_AnimationHolder->setSetting(setting.getSetting("animation"));
		m_FontHolder->setSetting(setting.getSetting("font"));
		m_SoundHolder->setSetting(setting.getSetting("sound"));
		m_MusicHolder->setSetting(setting.getSetting("music"));
		m_LightmapHolder->setSetting(setting.getSetting("texture"));

		//m_LuaScriptHolder->setSetting(setting.getSetting("script"));
		//m_ShaderHolder->setSetting(setting.getSetting("shader"));
	}


	void ResourceManager::loadDirectory(const std::string& directory)
	{
		m_TextureHolder->loadDirectory(getPath({directory, "texture"}));
		m_AnimationHolder->loadDirectory(getPath({directory, "animation"}));
		m_FontHolder->loadDirectory(getPath({directory, "font"}));
		m_SoundHolder->loadDirectory(getPath({directory, "sound"}));
		m_MusicHolder->loadDirectory(getPath({directory, "music"}));
		m_LightmapHolder->loadDirectory(getPath({directory, "lightmap"}));
		//m_LuaScriptHolder->loadDirectory(getPath({directory, "luascript"}));
		//m_ShaderHolder->loadDirectory(getPath({directory, "shader"}));
	}

	void ResourceManager::loadFile(ResourceType type, const std::string& file)
	{
		switch (type)
		{
			case ResourceType::Texture:
			{
				m_TextureHolder->loadFile(file);
			}break;

			case ResourceType::Animation:
			{
				m_AnimationHolder->loadFile(file);
			}break;

			case ResourceType::Font:
			{
				m_FontHolder->loadFile(file);
			}break;

			case ResourceType::Sound:
			{
				m_SoundHolder->loadFile(file);
			}break;

			case ResourceType::Music:
			{
				m_MusicHolder->loadFile(file);
			}break;

			case ResourceType::Lightmap:
			{
				m_LightmapHolder->loadFile(file);
				m_LightmapHolder->smoothAllTexture();
			}break;
		}
	}

	void ResourceManager::loadFile(ResourceType type, const std::vector<std::string> fileTable)
	{
		switch (type)
		{
			case ResourceType::Texture:
			{
				m_TextureHolder->loadFile(fileTable);
			}break;

			case ResourceType::Animation:
			{
				m_AnimationHolder->loadFile(fileTable);
			}break;

			case ResourceType::Font:
			{
				m_FontHolder->loadFile(fileTable);
			}break;

			case ResourceType::Sound:
			{
				m_SoundHolder->loadFile(fileTable);
			}break;

			case ResourceType::Music:
			{
				m_MusicHolder->loadFile(fileTable);
			}break;
		}
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

	TextureHolder::Ptr ResourceManager::getLightmapHolder()
	{
		return m_LightmapHolder;
	}
}
