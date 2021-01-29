////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/AdvancedScene.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class SceneManager
    {
        public:
                typedef std::shared_ptr<SceneManager> Ptr;

        public:
                                                SceneManager(Scene::Context context);
            //Scene Manager
            template <typename T>
            void                                addScene(std::string name);
            void                                setScene(std::string name);
            void                                buildScene();
            void                                loadScene(nlohmann::json scene);
            void                                loadScene(AdvancedScene::Ptr advanceScene, nlohmann::json scene);
            void                                firstLoad(nlohmann::json scene, const std::string& sceneName);
            nlohmann::json                      saveScene();
            nlohmann::json                      saveScene(AdvancedScene::Ptr advanceScene);
            nlohmann::json                      firstSave(const std::string& sceneName);
            void                                updateSceneSaveFile();
            void                                loadSceneSaveFile();
            void                                saveAllScene();

            const std::vector<sf::String>&      getSceneTable() const;
            const int&                          getSceneCount() const;

            Scene::Ptr                          getScene();
            sf::Vector2f                        getSceneResolution(Scene::Ptr scene);

            void                                setUpdateUI(std::function<void()>  fn);
            void                                setUpdateUndo(std::function<void()>  fn);
            void                                setUpdateLog(std::function<void(const std::string&, int)>  fn);
            void                                setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn);

            //Advanced scene game loop
            void                                update(const sf::Time& timeStep);
            void                                handleEvent(sf::Event& event);
            void                                render();
            void                                renderShape();
            void                                renderDebug();
            void                                renderFrontScreen();
            //Advanced scene
            SoundManager::Ptr                   getSoundManager();
            SceneSetting&                       getSceneSetting();
            //Advanced scene World
            SceneBuilder::Ptr                   getSceneBuilder();
            MeshEditor::Ptr                     getMeshEditor();
            UndoManager::Ptr                    getUndoManager();
            Grid::Ptr                           getWorldGrid();
            CameraSetting&                      getCameraSetting();
            //Advanced Scene screen
            SceneBuilder::Ptr                   getScreenBuilder();
            UndoManager::Ptr                    getScreenUndoManager();
            Grid::Ptr                           getScreenGrid();
            CameraSetting&                      getScreenCameraSetting();
            //Screen Management
            bool                                addScreen(const std::string& name);
            void                                selectScreen(const std::string& name);
            bool                                deleteScreen(const std::string& name);
            bool                                renameScreen(const std::string& name, const std::string& newName);
            std::vector<std::string>            getScreenTable();

        private:
            void                                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                                handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                                handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

        private:

            AdvancedScene::Ptr                  m_AdvancedScene;
            std::vector<sf::String>             m_SceneTable;
            Scene::Context                      m_Context;

            bool                                m_IsShiftOriginUp;
            bool                                m_IsShiftOriginDown;
            bool                                m_IsShiftOriginLeft;
            bool                                m_IsShiftOriginRight;
            bool                                m_IsLeftShift;
            bool                                m_IsMouseRightButton;
            b2Vec2                              m_LastMousePosition;
            b2Vec2                              m_ViewCenter;
            float                               m_ShitftOriginSpeed;

            std::function<void()>                               m_UpdateUI;
            std::function<void()>                               m_UpdateUndo;
            std::function<void(const std::string&, int)>        m_UpdateLog;
            std::function<void(const std::string&, bool, int)>  m_UpdateLogIf;
            std::map<std::string, std::pair<AdvancedScene::Ptr, std::function<Scene::Ptr()>>>   m_SceneFactoryMap;
    };

    template <typename T>
    void SceneManager::addScene(std::string sceneName)
    {
        //Create a AdvancedScene;
        m_SceneFactoryMap[sceneName].first = AdvancedScene::Ptr(new AdvancedScene(m_Context));
        m_SceneFactoryMap[sceneName].first->setName(sceneName);
        m_SceneTable.push_back(sceneName);

        m_SceneFactoryMap[sceneName].second = [this] ()
        {
            return Scene::Ptr(new T(m_Context));
        };

        //Create the scene directory if not exist
        createDirectory(WORKSPACE_FOLDER + "/" + sceneName);
        //Load the Scene if possible
        std::string file = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

        nlohmann::json scene;

        if(fileExist(file))
        {
            nero_log("save file detected : " + file);
            nero_log("loading scene");
            firstLoad(loadJson(file), sceneName);
        }
        else
        {
            nero_log("failed to find save file : " + file);
            nero_log("creating empty scene");
            scene = firstSave(sceneName);
            nero_log("creating save file");
            saveFile(file, scene.dump(3));
        }

        nero_log("scene registration complete");
    }
}
#endif // SCENEMANAGER_H
