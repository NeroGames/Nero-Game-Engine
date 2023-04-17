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
#include <Nero/editor/EditorCamera.h>
#include <Nero/editor/level/LevelBuilder.h>
#include <Nero/editor/screen/ScreenBuilder.h>
#include <Nero/editor/PhysicsInteractor.h>
// Std
#include <memory>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class AdvancedScene
    {
      public:
        using Ptr                        = std::shared_ptr<AdvancedScene>;
        using RenderTexturePtr           = std::shared_ptr<sf::RenderTexture>;
        using CreateCppGameScene         = GameScene::Ptr(GameScene::Context);
        using CreateCppGameLevel         = GameLevel::Ptr(GameLevel::Context);
        using CreateCppGameSceneCallback = boost::function<CreateCppGameScene>;
        using CreateCppGameLevelCallback = boost::function<CreateCppGameLevel>;

      public:
        AdvancedScene(Setting::Ptr projectSetting, const FontHolder::Ptr& editorFontHolder);

        void                     init();
        void                     clear();
        // level
        std::string              createLevel(const Parameter& parameter);
        void                     openLevel(const std::string& levelName);
        void                     closeSelectedLevel();
        bool                     removeLevel(const std::string& levelName);
        void                     registerLevel(const std::string& levelName);
        void                     unregisterLevel(const std::string& levelName);
        std::vector<std::string> getRegisteredLevelTable();
        // render
        void                     setRenderContext(const RenderContext::Ptr& renderContext);
        void                     setRenderTexture(const RenderTexturePtr& renderTexture);
        void                     setEditorCamera(const AdvancedCamera::Ptr& editorCamera);
        // Game Scene
        void                     handleEvent(const sf::Event& event);
        void                     update(const sf::Time& timeStep);
        void                     render();
        void                     renderShape();
        void                     renderFrontScreen();
        //
        void                     clearLoadedObject();
        void                     buildGameScene();
        LevelBuilder::Ptr        getLevelBuilder() const;
        //
        void                     setCreateSceneCallback(CreateCppGameSceneCallback callback);
        void                     registerCreateLevelCallback(const std::string          levelName,
                                                             CreateCppGameLevelCallback callback);

      private:
        Setting::Ptr                                      m_ProjectSetting;
        GameScene::Context                                m_SceneContext;
        GameScene::Ptr                                    m_GameScene;
        // Rendering
        AdvancedCamera::Ptr                               m_EditorCamera;
        RenderContext::Ptr                                m_RenderContext;
        std::shared_ptr<sf::RenderTexture>                m_RenderTexture;
        // Game Level
        LevelBuilder::Ptr                                 m_LevelBuilder;
        std::vector<std::string>                          m_RegisteredLevelTable;
        // DLL callback
        CreateCppGameSceneCallback                        m_CreateCppGameSceneCallback;
        std::map<std::string, CreateCppGameLevelCallback> m_CreateCppGameLevelCallbackMap;
        // Physics
        PhysicsInteractor::Ptr                            m_PhysicsInteractor;
    };
} // namespace nero
#endif // ADVANCEDSCENE_H
