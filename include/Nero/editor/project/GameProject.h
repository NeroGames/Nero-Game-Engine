////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEPROJECT_H
#define GAMEPROJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/editor/project/AdvancedScene.h>
#include <Nero/core/cpp/scene/GameScene.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/core/cpp/engine/BackgroundTask.h>
#include <Nero/editor/EditorCamera.h>
// Json
#include <json/json.hpp>
// Boost
#include <boost/function.hpp>
// STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class GameProject
    {
      public:
        using Ptr = std::shared_ptr<GameProject>;

      public:
        GameProject(Setting::Ptr projectSetting = nullptr);

        void               init();
        bool               loadLibrary();
        void               openEditor();
        void               closeProject();
        // Getter
        AdvancedScene::Ptr getAdvancedScene();
        std::string        getProjectName() const;
        std::string        getProjectDirectory() const;

        // Static
        static void        compileProject(const std::string&        projectDirectory,
                                          const BackgroundTask::Ptr backgroundTask);

      private:
        void openQtCreator(const std::string& file = StringPool.BLANK);
        void openVisualStudio(const std::string& file = StringPool.BLANK);

      private:
        AdvancedScene::Ptr m_AdvancedScene;
        Setting::Ptr       m_ProjectSetting;
        std::string        m_CodeEditorProcessId;
    };
} // namespace nero

#endif // GAMEPROJECT_H
