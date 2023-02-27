////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORPROXY_H
#define EDITORPROXY_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/engine/Parameter.h>
// Std
#include <functional>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorProxy
    {
      public:
        using Ptr = std::shared_ptr<EditorProxy>;

      public:
        EditorProxy();

        // Game Project
        void openProject(const std::string& projectDirectory) const;
        void createProject(const Parameter&    projectParameter,
                           const unsigned int& projectCount) const;
        void saveProject() const;
        void closeProject() const;
        void loadProject() const;
        void openCodeEditor() const;
        void compileProject() const;
        void reloadProjectLibrary() const;
        // Workspace
        void createWorkspace(const Parameter& workspaceParameter) const;
        void importWorkspace(const std::string& workspaceDirectory) const;
        // Editor
        void closeEditor() const;
        // Game Level
        void createGameLevel(const Parameter& levelParameter) const;
        void openGameLevel(const std::string levelName) const;
        void removeGameLevel(const std::string levelName) const;
        // Play Mode
        void playGameScene() const;
        void stopPlayGameScene() const;
        // Auto save
        void autoSave() const;

      private:
        friend class EditorUI;
        // Game Project
        std::function<void(const std::string&)>                    m_OpenProjectCallback;
        std::function<void(const Parameter&, const unsigned int&)> m_CreateProjectCallback;
        std::function<void()>                                      m_SaveProjectCallback;
        std::function<void()>                                      m_CloseProjectCallback;
        std::function<void()>                                      m_OpenCodeEditorCallback;
        std::function<void()>                                      m_CompileProjectCallback;
        std::function<void()>                                      m_ReloadProjectLibraryCallback;
        // Workspace
        std::function<void(const Parameter&)>                      m_CreateWorkspaceCallback;
        std::function<void(const std::string&)>                    m_ImportWorkspaceCallback;
        // Editor
        std::function<void()>                                      m_CloseEditorCallback;
        // Game Level
        std::function<void(const Parameter&)>                      m_CreateGameLevelCallback;
        std::function<void(const std::string&)>                    m_OpenGameLevelCallback;
        std::function<void(const std::string&)>                    m_RemoveGameLevelCallback;
        // Game Scene
        std::function<void()>                                      m_PlayGameSceneCallback;
        std::function<void()>                                      m_StopPlayGameSceneCallback;
        // Auto Save
        std::function<void()>                                      m_AutoSaveCallback;
    };
} // namespace nero

#endif // EDITORPROXY_H
