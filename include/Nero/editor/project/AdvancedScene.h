////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/engine/Parameter.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/core/cpp/scene/GameScene.h>
#include <Nero/editor/level/LevelBuilder.h>
#include <Nero/editor/screen/ScreenBuilder.h>
// STD
#include <memory>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class AdvancedScene
    {
      public:
        using Ptr              = std::shared_ptr<AdvancedScene>;
        using RenderTexturePtr = std::shared_ptr<sf::RenderTexture>;

      public:
        AdvancedScene();

        void                     init();
        void                     clear();
        // level
        std::string              createLevel(const Parameter& parameter);
        void                     openLevel(const std::string& levelName);
        void                     closeSelectedLevel();
        void                     removeLevel(const std::string& levelName);
        void                     registerLevel(const std::string& levelName);
        void                     unregisterLevel(const std::string& levelName);
        std::vector<std::string> getRegisteredLevelTable();
        // screen
        void                     createScreen(const Parameter& parameter);
        // setting
        void                     setEditorSetting(const Setting::Ptr& setting);
        void                     setProjectSetting(const Setting::Ptr& setting);
        // render
        void                     setRenderContext(const RenderContext::Ptr& renderContext);
        void                     setRenderTexture(const RenderTexturePtr& renderTexture);
        // Game Scene
        void                     setGameScene(GameScene::Ptr gameScene);
        void                     handleEvent(const sf::Event& event);
        void                     update(const sf::Time& timeStep);
        void                     render();
        void                     renderShape();
        //
        void                     clearGameSceneObject();
        void                     buildGameScene();

        LevelBuilder::Ptr        getLevelBuilder() const;
        void registerLevelClass(const std::string levelName, GameLevel::Ptr gameLevel);

      private:
        // level
        LevelBuilder::Ptr                     m_LevelBuilder;
        std::vector<std::string>              m_RegisteredLevelTable;
        // screen
        ScreenBuilder::Ptr                    m_SelectedScreen;
        // setting
        Setting::Ptr                          m_EditorSetting;
        Setting::Ptr                          m_ProjectSetting;
        Setting::Ptr                          m_SceneSetting;
        // render
        RenderContext::Ptr                    m_RenderContext;
        std::shared_ptr<sf::RenderTexture>    m_RenderTexture;
        //
        GameScene::Ptr                        m_GameScene;
        std::map<std::string, GameLevel::Ptr> m_GameLevelMap;
    };
} // namespace nero
#endif // ADVANCEDSCENE_H
