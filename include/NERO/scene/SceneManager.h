////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <NERO/scene/DevScene.h>

namespace nero
{

    class SceneManager
    {
        public:
                                            SceneManager(Scene::Context context);

            void                            update();
            void                            handleEvent(sf::Event& event);

            void                            render();
            void                            renderShape();
            void                            renderDebug();
            void                            renderOnFrontScreen();

            std::vector<sf::String>         getSceneList();
            int                             getSceneCount();

            template <typename T>
            void                            registerScene(sf::String name);

            void                            setScene(sf::String name);

            void                            restartScene();

            b2World*                        getSceneWorld() const;
            ObjectManager*                  getObjectManager();
            ObjectManager*                  getObjectManager(std::string name);
            UndoManager*                    getUndoManager();
            UndoManager*                    getUndoManager(std::string name);
            std::string                     getSceneName();

            void                            setCameraSettings(CameraSettings cameraSettings);
            CameraSettings&                 getCameraSetting();

            MeshEditor*                     getMeshEditor();

            SceneSettings&                  getSceneSettings();
            Object::Ptr                     getRootObject();
            void                            buildScene();


    private:
            void                            handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                            handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                            handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);


        private:
            SceneSettings                   m_SceneSettings;

            bool                            m_IsShiftOriginUp;
            bool                            m_IsShiftOriginDown;
            bool                            m_IsShiftOriginLeft;
            bool                            m_IsShiftOriginRight;

            bool                            m_IsLeftShift;
            bool                            m_IsMouseRightButton;

            b2Vec2                          m_LastMousePosition;
            b2Vec2                          m_ViewCenter;

            float                           m_ShitftOriginSpeed;

            std::vector<sf::String>         m_SceneList;

            Scene::Context                  m_Context;
            DevScene::Ptr                   m_DevScene;
            sf::String                      m_SceneName;

            std::map<sf::String, std::pair<DevScene::Ptr, std::function<Scene::Ptr()>>>     m_Factories;

    };

    template <typename T>
    void SceneManager::registerScene(sf::String name)
    {
        //Create a DevScene;
        m_Factories[name].first = DevScene::Ptr(new DevScene(m_Context));
        m_Factories[name].first->setName(name);
        m_SceneList.push_back(name);

        m_Factories[name].second = [this] ()
        {
            return Scene::Ptr(new T(m_Context));
        };
    }
}

#endif // SCENEMANAGER_H
