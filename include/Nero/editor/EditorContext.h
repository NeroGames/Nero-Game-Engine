////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORCONTEXT_H
#define EDITORCONTEXT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceUtility.h>
#include <Nero/editor/EditorProxy.h>
#include <Nero/editor/EditorUtility.h>
#include <Nero/editor/project/ProjectManager.h>
#include <Nero/editor/NotificationManager.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorContext
    {
      public:
        using Ptr              = std::shared_ptr<EditorContext>;
        using RenderTexturePtr = std::shared_ptr<sf::RenderTexture>;

      public:
        EditorContext(EditorProxy::Ptr         editorProxy,
                      ProjectManager::Ptr      projectManager,
                      NotificationManager::Ptr notificationManager,
                      TextureHolder::Ptr       textureHolder,
                      FontHolder::Ptr          fontHolder,
                      Setting::Ptr             editorSetting,
                      RenderTexturePtr         renderTexture,
                      RenderTexturePtr         frontScreenTexture,
                      RenderContext::Ptr       renderContext,
                      EditorCamera::Ptr      editorCamara,
                      const EditorMode&        editorMode  = EditorMode::None,
                      const BuilderMode&       builderMode = BuilderMode::None);
        virtual ~EditorContext();

        // Getter
        EditorProxy::Ptr         getEditorProxy() const;
        ProjectManager::Ptr      getProjectManager() const;
        NotificationManager::Ptr getNotificationManager() const;
        GameProject::Ptr         getGameProject() const;
        AdvancedScene::Ptr       getAdvancedScene() const;
        LevelBuilder::Ptr        getLevelBuilder() const;
        EditorMode               getEditorMode() const;
        BuilderMode              getBuilderMode() const;
        TextureHolder::Ptr       getTextureHolder() const;
        FontHolder::Ptr          getFontHolder() const;
        Setting::Ptr             getEditorSetting() const;
        std::string              getSelectedGameLevelName() const;
        std::string              getOpengedGameLevelName() const;
        ResourceType             getSelectedResourceType() const;
        ResourceManager::Ptr     getCurrentResourceManager() const;
        sf::Vector2f             getNewGameObjectPosition() const;
        RenderTexturePtr         getRenderTexture() const;
        RenderTexturePtr         getFrontScreenTexture() const;
        RenderContext::Ptr       getRenderContext() const;
        EditorCamera::Ptr      getEditorCamera() const;
        float                    getFrameRate() const;
        float                    getFrameTime() const;
        bool                     autoSaveEnabled() const;

        // Setter
        void                     setEditorMode(const EditorMode& editorMode);
        void                     setBuilderMode(const BuilderMode& builderMode);
        void                     setSelectedGameLevelName(const std::string& levelName);
        void                     setOpenedGameLevelName(const std::string& levelName);
        void                     setSelectedResourceType(const ResourceType& resourceType);
        void                     setFrameRate(const float& frameRate);
        void                     setFrameTime(const float& frameTime);
        void                     setAutoSaveEnabled(const bool& enabled);

      private:
        EditorProxy::Ptr         m_EditorProxy;
        ProjectManager::Ptr      m_ProjectManager;
        NotificationManager::Ptr m_NotificationManager;
        TextureHolder::Ptr       m_TextureHolder;
        FontHolder::Ptr          m_FontHolder;
        Setting::Ptr             m_EditorSetting;
        RenderTexturePtr         m_RenderTexture;
        RenderTexturePtr         m_FrontScreenTexture;
        RenderContext::Ptr       m_RenderContext;
        EditorCamera::Ptr      m_EditorCamera;
        EditorMode               m_EditorMode;
        BuilderMode              m_BuilderMode;
        // Game Level
        std::string              m_SelectedGameLevelName;
        std::string              m_OpenedGameLevelName;
        // Resource
        ResourceType             m_SelectedResourceType;
        // Frame Rate
        float                    m_FrameRate;
        float                    m_FrameTime;
        // Auto Save
        bool                     m_AutoSaveEnabled;
    };
} // namespace nero

#endif // EDITORCONTEXT_H
