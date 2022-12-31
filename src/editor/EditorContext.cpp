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

    void EditorContext::setEditorMode(const EditorMode& editorMode)
    {
        m_EditorMode = editorMode;
    }

    void EditorContext::setBuilderMode(const BuilderMode& builderMode)
    {
        m_BuilderMode = builderMode;
    }
}
