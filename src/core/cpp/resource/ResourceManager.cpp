////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////

///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/Logging.h>
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
		m_TextureHolder->loadDirectory(file::getPath({directory, "texture"}));
		m_AnimationHolder->loadDirectory(file::getPath({directory, "animation"}));
		m_FontHolder->loadDirectory(file::getPath({directory, "font"}));
		m_SoundHolder->loadDirectory(file::getPath({directory, "sound"}));
		m_MusicHolder->loadDirectory(file::getPath({directory, "music"}));
		m_LightmapHolder->loadDirectory(file::getPath({directory, "lightmap"}));
		//m_LuaScriptHolder->loadDirectory(getPath({directory, "luascript"}));
		//m_ShaderHolder->loadDirectory(getPath({directory, "shader"}));
	}

	bool ResourceManager::loadFile(ResourceType type, const std::string& file)
	{
		switch (type)
		{
			case ResourceType::Texture:
			{
				return m_TextureHolder->loadFile(file);
			}

			case ResourceType::Animation:
			{
				return m_AnimationHolder->loadFile(file);
			}

			case ResourceType::Font:
			{
				return m_FontHolder->loadFile(file);
			}

			case ResourceType::Sound:
			{
				return m_SoundHolder->loadFile(file);
			}

			case ResourceType::Music:
			{
				return m_MusicHolder->loadFile(file);
			}

			case ResourceType::Lightmap:
			{
				bool added = m_LightmapHolder->loadFile(file);
				if(added)
				{
					m_LightmapHolder->smoothAllTexture();
				}

				return added;
			}
		}

		return false;
	}

	const std::vector<std::string> ResourceManager::loadFile(ResourceType type, const std::vector<std::string> fileTable)
	{
		switch (type)
		{
			case ResourceType::Texture:
			{
				return m_TextureHolder->loadFile(fileTable);
			}

			case ResourceType::Animation:
			{
				return m_AnimationHolder->loadFile(fileTable);
			}

			case ResourceType::Font:
			{
				return m_FontHolder->loadFile(fileTable);
			}

			case ResourceType::Sound:
			{
				return m_SoundHolder->loadFile(fileTable);
			}

			case ResourceType::Music:
			{
				return m_MusicHolder->loadFile(fileTable);
			}

			case ResourceType::Lightmap:
			{
				return m_LightmapHolder->loadFile(fileTable);
			}
		}

		return std::vector<std::string>();
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

	void ResourceManager::clearResource()
	{
		nero_log("clearing font resource");
		m_FontHolder->clear();

		nero_log("clearing sound resource");
		m_SoundHolder->clear();

		nero_log("clearing music resource");
		m_MusicHolder->clear();

		nero_log("clearing shader resource");
		//m_ShaderHolder->clear();

		nero_log("clearing lua script resource");
		//m_LuaScriptHolder->clear();

		nero_log("clearing texture resource");
		m_TextureHolder->clear();

		nero_log("clearing animation resource");
		m_AnimationHolder->clear();

		nero_log("clearing texture resource");
		m_TextureHolder->clear();
	}

	void ResourceManager::loadRequired(const nlohmann::json& requiredResource)
	{

	}

	void ResourceManager::buildDirectory(const std::string& parentDirectory)
	{
		file::createDirectory(file::getPath({parentDirectory, "texture"}));
		file::createDirectory(file::getPath({parentDirectory, "animation"}));
		file::createDirectory(file::getPath({parentDirectory, "font"}));
		file::createDirectory(file::getPath({parentDirectory, "sound"}));
		file::createDirectory(file::getPath({parentDirectory, "music"}));
		file::createDirectory(file::getPath({parentDirectory, "luascript"}));
		file::createDirectory(file::getPath({parentDirectory, "shader"}));
		file::createDirectory(file::getPath({parentDirectory, "language"}));
		file::createDirectory(file::getPath({parentDirectory, "lightmap"}));
	}


}
