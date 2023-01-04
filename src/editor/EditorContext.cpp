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
                                 const EditorMode& editorMode,
                                 const BuilderMode& builderMode):
         m_EditorProxy(editorProxy)
        ,m_ProjectManager(projectManager)
        ,m_TextureHolder(textureHolder)
        ,m_EditorSetting(editorSetting)
        ,m_EditorMode(editorMode)
        ,m_BuilderMode(builderMode)
        ,m_SelectedGameLevelName(StringPool.BLANK)
        ,m_OpenedGameLevelName(StringPool.BLANK)
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
        if(m_ProjectManager)
        {
            return m_ProjectManager->getGameProject();
        }

        return nullptr;
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
}
