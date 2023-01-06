////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/EditorContext.h>
////////////////////////////////////////////////////////////
namespace  nero
{
    EditorContext::EditorContext(EditorProxy::Ptr editorProxy,
                                 ProjectManager::Ptr projectManager,
                                 TextureHolder::Ptr textureHolder,
                                 Setting::Ptr editorSetting,
                                 RenderTexturePtr renderTexture,
                                 RenderContext::Ptr renderContext,
                                 const EditorMode& editorMode,
                                 const BuilderMode& builderMode)
        :m_EditorProxy(editorProxy)
        ,m_ProjectManager(projectManager)
        ,m_TextureHolder(textureHolder)
        ,m_EditorSetting(editorSetting)
        ,m_RenderTexture(renderTexture)
        ,m_RenderContext(renderContext)
        ,m_EditorMode(editorMode)
        ,m_BuilderMode(builderMode)
        ,m_SelectedGameLevelName(StringPool.BLANK)
        ,m_OpenedGameLevelName(StringPool.BLANK)
        ,m_SelectedResourceType(ResourceType::None)
    {

    }

    EditorContext::~EditorContext()
    {

    }

    EditorProxy::Ptr EditorContext::getEditorProxy() const
    {
        return m_EditorProxy;
    }

    ProjectManager::Ptr EditorContext::getProjectManager() const
    {
        return m_ProjectManager;
    }

    GameProject::Ptr EditorContext::getGameProject() const
    {
        if(!m_ProjectManager)
            return nullptr;

       return m_ProjectManager->getGameProject();
    }

    AdvancedScene::Ptr EditorContext::getAdvancedScene() const
    {
        GameProject::Ptr gameProject = getGameProject();

        if(!gameProject)
            return nullptr;

        return gameProject->getAdvancedScene();
    }

    LevelBuilder::Ptr EditorContext::getLevelBuilder() const
    {
        AdvancedScene::Ptr advancedScene = getAdvancedScene();

        if(!advancedScene)
            return nullptr;

        return advancedScene->getLevelBuilder();
    }

    TextureHolder::Ptr EditorContext::getTextureHolder() const
    {
        return m_TextureHolder;
    }

    Setting::Ptr EditorContext::getEditorSetting() const
    {
        return m_EditorSetting;
    }

    EditorMode EditorContext::getEditorMode() const
    {
        return m_EditorMode;
    }

    BuilderMode EditorContext::getBuilderMode() const
    {
        return m_BuilderMode;
    }

    std::string EditorContext::getSelectedGameLevelName()  const
    {
        return m_SelectedGameLevelName;
    }

    std::string EditorContext::getOpengedGameLevelName()  const
    {
        return m_OpenedGameLevelName;
    }

    ResourceType EditorContext::getSelectedResourceType() const
    {
        return m_SelectedResourceType;
    }

    sf::Vector2f EditorContext::getNewGameObjectPosition() const
    {
        sf::Vector2f screenPosition = sf::Vector2f(m_RenderContext->canvasSize.x / 2.f, 150.f);
        return m_RenderTexture->mapPixelToCoords(sf::Vector2i(screenPosition.x, screenPosition.y),
                                                 m_RenderTexture->getView());
    }

    void EditorContext::setEditorMode(const EditorMode& editorMode)
    {
        m_EditorMode = editorMode;
    }

    void EditorContext::setBuilderMode(const BuilderMode& builderMode)
    {
        m_BuilderMode = builderMode;
    }

    void EditorContext::setSelectedGameLevelName(const std::string& levelName)
    {
        m_SelectedGameLevelName = levelName;
    }

    void EditorContext::setOpenedGameLevelName(const std::string& levelName)
    {
        m_OpenedGameLevelName = levelName;
    }

    void EditorContext::setSelectedResourceType(const ResourceType& resourceType)
    {
        m_SelectedResourceType = resourceType;
    }

    ResourceManager::Ptr EditorContext::getCurrentResourceManager() const
    {
        switch (m_EditorMode)
        {
            case EditorMode::WORLD_BUILDER:
            {
                auto levelBuilder = getLevelBuilder();

                if(!levelBuilder)
                    return nullptr;

                return levelBuilder->getResourceManager();
            }
            default:
            {
                // TODO
                return nullptr;
            }
        }
    }
 }
