////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GAMEPROJECT_H
#define GAMEPROJECT_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/utility/CommandLine.h>
#include <Nero/editor/project/AdvancedScene.h>
#include <Nero/core/cpp/scene/Scene.h>
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
        using Ptr                        = std::shared_ptr<GameProject>;
        using RenderTexturePtr           = std::shared_ptr<sf::RenderTexture>;
        using CreateCppGameSceneCallback = Scene::Ptr(Scene::Context);
        using CreateCppGameLevelCallback = GameLevel::Ptr(GameLevel::Context);
        // TODO
        // using CreateCppGameScreenCallback = GameScreen::Ptr(GameScreen::Context);

      public:
        GameProject(Setting::Ptr projectSetting = nullptr);

        void               init();
        void               loadLibrary();
        void               openEditor();
        void               closeProject();
        // Setter
        void               setRenderTexture(const RenderTexturePtr& renderTexture);
        void               setRenderContext(const RenderContext::Ptr& renderContext);
        void               setCamera(const Camera::Ptr& camera);
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
        AdvancedScene::Ptr                          m_AdvancedScene;
        Setting::Ptr                                m_ProjectSetting;
        // User defined class
        boost::function<CreateCppGameSceneCallback> m_CreateCppGameSceneCallback;
        boost::function<CreateCppGameLevelCallback> m_CreateCppGameLevelCallback;
        // Open code editor
        std::string                                 m_CodeEditorProcessId;
    };
} // namespace nero

#endif // GAMEPROJECT_H
