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
////////////////////////////////////////////////////////////
namespace nero
{
	class ResourceManager
    {
		public: //utility
			typedef std::shared_ptr<ResourceManager>    Ptr;

		public:
										ResourceManager();
									   ~ResourceManager();

			void						addDirectory();
			void						loadResource();
			void						reloadResource();
			void						clearResource();

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
    };
}
#endif // RESOURCEMANAGER_H
