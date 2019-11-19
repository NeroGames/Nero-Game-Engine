////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/FontHolder.h>
#include <Nero/core/resource/SoundHolder.h>
#include <Nero/core/resource/MusicHolder.h>
#include <Nero/core/resource/ShaderHolder.h>
#include <Nero/core/resource/ScriptHolder.h>
#include <Nero/core/resource/TextureHolder.h>
#include <Nero/core/resource/AnimationHolder.h>
#include <Nero/core/resource/ResourceUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
	class ResourceManager
    {
		public: //utility
			typedef std::shared_ptr<ResourceManager>    Ptr;

		public:
										ResourceManager();
										ResourceManager(const Setting& setting);
									   ~ResourceManager();

			void						setSetting(const Setting& setting);

			void						loadDirectory(const std::string& directory);
			void						loadFile(ResourceType type, const std::string& file);
			void						loadFile(ResourceType type, const std::vector<std::string> fileTable);

			//void						loadDirectory();
			//void						addDirectory(const Setting& setting, bool autoSelect = false);
			//void						loadResource();
			//void						reloadResource();
			//void						clearResource();

			void						destroy();

			FontHolder::Ptr				getFontHolder();
			SoundHolder::Ptr			getSoundHolder();
			MusicHolder::Ptr			getMusicHolder();
			ShaderHolder::Ptr			getShaderHolder();
			ScriptHolder::Ptr			getLuaScriptHolder();
			TextureHolder::Ptr			getTextureHolder();
			AnimationHolder::Ptr		getAnimationHolder();

		private:
			FontHolder::Ptr				m_FontHolder;
			SoundHolder::Ptr			m_SoundHolder;
			MusicHolder::Ptr			m_MusicHolder;
			ShaderHolder::Ptr			m_ShaderHolder;
			ScriptHolder::Ptr			m_LuaScriptHolder;
			TextureHolder::Ptr			m_TextureHolder;
			AnimationHolder::Ptr		m_AnimationHolder;

			Setting						m_Setting;
    };
}
#endif // RESOURCEMANAGER_H
