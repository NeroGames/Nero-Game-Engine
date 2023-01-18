////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/resource/FontHolder.h>
#include <Nero/core/cpp/resource/SoundHolder.h>
#include <Nero/core/cpp/resource/MusicHolder.h>
#include <Nero/core/cpp/resource/ShaderHolder.h>
#include <Nero/core/cpp/resource/ScriptHolder.h>
#include <Nero/core/cpp/resource/TextureHolder.h>
#include <Nero/core/cpp/resource/AnimationHolder.h>
#include <Nero/core/cpp/resource/ResourceUtility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class ResourceManager {
      public: // utility
        typedef std::shared_ptr<ResourceManager> Ptr;

      public:
        ResourceManager();
        ResourceManager(const Setting& setting);
        ~ResourceManager();

        void                           setSetting(const Setting& setting);

        void                           loadDirectory(const std::string& directory);
        bool                           loadFile(ResourceType type, const std::string& file);
        const std::vector<std::string> loadFile(ResourceType type, const std::vector<std::string> fileTable);
        void                           loadRequired(const nlohmann::json& requiredResource);

        // void						loadDirectory();
        // void						addDirectory(const Setting& setting, bool autoSelect = false);
        // void						loadResource();
        // void						reloadResource();
        void                           clearResource();

        void                           destroy();

        FontHolder::Ptr                getFontHolder();
        SoundHolder::Ptr               getSoundHolder();
        MusicHolder::Ptr               getMusicHolder();
        ShaderHolder::Ptr              getShaderHolder();
        ScriptHolder::Ptr              getLuaScriptHolder();
        TextureHolder::Ptr             getTextureHolder();
        AnimationHolder::Ptr           getAnimationHolder();
        TextureHolder::Ptr             getLightmapHolder();

      public:
        static void buildDirectory(const std::string& parentDirectory = StringPool.BLANK);

      private:
        FontHolder::Ptr      m_FontHolder;
        SoundHolder::Ptr     m_SoundHolder;
        MusicHolder::Ptr     m_MusicHolder;
        ShaderHolder::Ptr    m_ShaderHolder;
        ScriptHolder::Ptr    m_LuaScriptHolder;
        TextureHolder::Ptr   m_TextureHolder;
        AnimationHolder::Ptr m_AnimationHolder;
        TextureHolder::Ptr   m_LightmapHolder;

        Setting              m_Setting;
    };
} // namespace nero
#endif // RESOURCEMANAGER_H
