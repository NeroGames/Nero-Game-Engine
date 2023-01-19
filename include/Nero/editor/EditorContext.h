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
        EditorContext(EditorProxy::Ptr    editorProxy,
                      ProjectManager::Ptr projectManager,
                      TextureHolder::Ptr  textureHolder,
                      FontHolder::Ptr     fontHolder,
                      Setting::Ptr        editorSetting,
                      RenderTexturePtr    renderTexture,
                      RenderContext::Ptr  renderContext,
                      AdvancedCamera::Ptr editorCamara,
                      const EditorMode&   editorMode  = EditorMode::WORLD_BUILDER,
                      const BuilderMode&  builderMode = BuilderMode::OBJECT);
        virtual ~EditorContext();

        // Getter
        EditorProxy::Ptr     getEditorProxy() const;
        ProjectManager::Ptr  getProjectManager() const;
        GameProject::Ptr     getGameProject() const;
        AdvancedScene::Ptr   getAdvancedScene() const;
        LevelBuilder::Ptr    getLevelBuilder() const;
        EditorMode           getEditorMode() const;
        BuilderMode          getBuilderMode() const;
        TextureHolder::Ptr   getTextureHolder() const;
        FontHolder::Ptr      getFontHolder() const;
        Setting::Ptr         getEditorSetting() const;
        std::string          getSelectedGameLevelName() const;
        std::string          getOpengedGameLevelName() const;
        ResourceType         getSelectedResourceType() const;
        ResourceManager::Ptr getCurrentResourceManager() const;
        sf::Vector2f         getNewGameObjectPosition() const;
        RenderTexturePtr     getRenderTexture() const;
        RenderContext::Ptr   getRenderContext() const;
        AdvancedCamera::Ptr  getEditorCamera() const;
        float                getFrameRate() const;
        float                getFrameTime() const;

        // Setter
        void                 setEditorMode(const EditorMode& editorMode);
        void                 setBuilderMode(const BuilderMode& builderMode);
        void                 setSelectedGameLevelName(const std::string& levelName);
        void                 setOpenedGameLevelName(const std::string& levelName);
        void                 setSelectedResourceType(const ResourceType& resourceType);
        void                 setFrameRate(const float& frameRate);
        void                 setFrameTime(const float& frameTime);

      private:
        EditorProxy::Ptr    m_EditorProxy;
        ProjectManager::Ptr m_ProjectManager;
        TextureHolder::Ptr  m_TextureHolder;
        FontHolder::Ptr     m_FontHolder;
        Setting::Ptr        m_EditorSetting;
        RenderTexturePtr    m_RenderTexture;
        RenderContext::Ptr  m_RenderContext;
        AdvancedCamera::Ptr m_EditorCamera;
        EditorMode          m_EditorMode;
        BuilderMode         m_BuilderMode;
        // Game Level
        std::string         m_SelectedGameLevelName;
        std::string         m_OpenedGameLevelName;
        // Resource
        ResourceType        m_SelectedResourceType;
        // Frame Rate
        float               m_FrameRate;
        float               m_FrameTime;
    };
} // namespace nero

#endif // EDITORCONTEXT_H
