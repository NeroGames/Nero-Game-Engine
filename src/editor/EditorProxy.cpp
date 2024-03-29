////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/EditorProxy.h>
////////////////////////////////////////////////////////////
namespace nero
{
    EditorProxy::EditorProxy()
    {
    }

    void EditorProxy::openProject(const std::string& projectDirectory) const
    {
        m_OpenProjectCallback(projectDirectory);
    }

    void EditorProxy::createProject(const Parameter&    projectParameter,
                                    const unsigned int& projectCount) const
    {
        m_CreateProjectCallback(projectParameter, projectCount);
    }

    void EditorProxy::createWorkspace(const Parameter& workspaceParameter) const
    {
        m_CreateWorkspaceCallback(workspaceParameter);
    }

    void EditorProxy::importWorkspace(const std::string& workspaceDirectory) const
    {
        m_ImportWorkspaceCallback(workspaceDirectory);
    }

    void EditorProxy::saveProject() const
    {
        m_SaveProjectCallback();
    }

    void EditorProxy::closeProject() const
    {
        m_CloseProjectCallback();
    }

    void EditorProxy::loadProject() const
    {
        // TODO
    }

    void EditorProxy::closeEditor() const
    {
        m_CloseEditorCallback();
    }

    void EditorProxy::createGameLevel(const Parameter& levelParameter) const
    {
        m_CreateGameLevelCallback(levelParameter);
    }

    void EditorProxy::openGameLevel(const std::string levelName) const
    {
        m_OpenGameLevelCallback(levelName);
    }

    void EditorProxy::removeGameLevel(const std::string levelName) const
    {
        m_RemoveGameLevelCallback(levelName);
    }

    void EditorProxy::openCodeEditor() const
    {
        m_OpenCodeEditorCallback();
    }

    void EditorProxy::compileProject() const
    {
        m_CompileProjectCallback();
    }

    void EditorProxy::reloadProjectLibrary() const
    {
        m_ReloadProjectLibraryCallback();
    }

    void EditorProxy::playGameScene() const
    {
        m_PlayGameSceneCallback();
    }

    void EditorProxy::stopPlayGameScene() const
    {
        m_StopPlayGameSceneCallback();
    }

    void EditorProxy::autoSave() const
    {
        m_AutoSaveCallback();
    }
} // namespace nero
