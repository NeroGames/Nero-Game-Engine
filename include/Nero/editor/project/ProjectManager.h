////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/editor/project/GameProject.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/editor/EditorCamera.h>
#include <Nero/core/cpp/resource/FontHolder.h>
// Json
#include <json/json.hpp>
// Boost
#include <boost/function.hpp>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class ProjectManager
    {
      public:
        using Ptr              = std::shared_ptr<ProjectManager>;
        using RenderTexturePtr = std::shared_ptr<sf::RenderTexture>;

      public:
        // Constructor
        ProjectManager(Setting::Ptr editorSetting, FontHolder::Ptr editorFontHolder);
        ~ProjectManager();

        // Game Project
        void createProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
        void openProject(const std::string& projectDirectory);
        void closeProject();
        bool projectExist(const std::string& projectName, const std::string& workspaceName);
        const std::vector<nlohmann::json> getWorkspaceProjectTable(const std::string& workpsace);
        nlohmann::json                    findProject(const std::string& workspace_name,
                                                      const std::string& project_name);
        void                              updateRecentProject(const std::string& projectDirectory);
        GameProject::Ptr                  getGameProject();
        // Workspace
        void                              createWorkspace(const Parameter& parameter);
        void                              importWorkspace(const std::string& directory);
        const nlohmann::json              getWorkspaceTable() const;
        const std::vector<std::string>    getWorkspaceNameTable() const;
        const nlohmann::json              findWorkspace(const std::string& workspaceName) const;
        bool                              workspaceExist(const std::string& workspaceName);
        std::string                       getProjectDirectory(const Parameter& parameter);

      private:
        // Game Project
        void createCppProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
        void createLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
        void createCppLuaProject(const Parameter& parameter, BackgroundTask::Ptr backgroundTask);
        // Other
        std::string formatSceneClassName(std::vector<std::string> wordTable);
        std::string formatHeaderGard(std::vector<std::string> wordTable);
        std::string formatCmakeProjectName(std::vector<std::string> wordTable);
        std::string formatCmakeProjectLibrary(std::vector<std::string> wordTable);

      private:
        Setting::Ptr     m_EditorSetting;
        GameProject::Ptr m_GameProject;
        FontHolder::Ptr  m_EditorFontHolder;
    };
} // namespace nero

#endif // PROJECTMANAGER_H
