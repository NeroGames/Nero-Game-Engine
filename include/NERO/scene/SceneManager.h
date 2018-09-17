////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
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
                                            SceneManager(Context context);

            void                            update(const sf::Time& timeStep);
            void                            handleEvent(sf::Event& event);

            void                            render();
            void                            renderShape();
            void                            renderDebug();
            void                            renderOnFrontScreen();

            std::vector<sf::String>         getSceneTable();
            int                             getSceneCount();

            template <typename T>
            void                            addScene(std::string name);
            void                            setScene(std::string name);
            void                            restartScene();
            void                            buildScene();

            b2World*                        getPhysicWorld() const;
            MeshEditor::Ptr                 getMeshEditor();
            SceneBuilder::Ptr               getSceneBuilder();
            UndoManager::Ptr                getUndoManager();
            SoundManager::Ptr               getSoundManager();
            SceneBuilder::Ptr               getSceneBuilder(std::string name);
            UndoManager::Ptr                getUndoManager(std::string name);

            Object::Ptr                     getRootObject();
            std::string                     getSceneName();
            CameraSetting&                  getCameraSetting();
            SceneSetting&                   getSceneSetting();

        private:
            void                            handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                            handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                            handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

        private:

            std::map<sf::String, std::pair<AdvancedScene::Ptr, std::function<Scene::Ptr()>>>     m_SceneFactoryMap;

            AdvancedScene::Ptr              m_AdvancedScene;
            std::vector<sf::String>         m_SceneTable;
            Context                         m_Context;

            bool                            m_IsShiftOriginUp;
            bool                            m_IsShiftOriginDown;
            bool                            m_IsShiftOriginLeft;
            bool                            m_IsShiftOriginRight;
            bool                            m_IsLeftShift;
            bool                            m_IsMouseRightButton;
            b2Vec2                          m_LastMousePosition;
            b2Vec2                          m_ViewCenter;
            float                           m_ShitftOriginSpeed;

    };

    template <typename T>
    void SceneManager::addScene(std::string name)
    {
        //Create a AdvancedScene;
        m_SceneFactoryMap[name].first = AdvancedScene::Ptr(new AdvancedScene(m_Context));
        m_SceneFactoryMap[name].first->setName(name);
        m_SceneTable.push_back(name);

        m_SceneFactoryMap[name].second = [this] ()
        {
            return Scene::Ptr(new T(m_Context));
        };
    }
}

#endif // SCENEMANAGER_H
