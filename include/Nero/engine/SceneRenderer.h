////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
#ifndef SCENERENDERER_H
#define SCENERENDERER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/engine/Engine.h>
#include <Nero/scene/Scene.h>
#include <Nero/camera/Camera.h>
#include <Nero/scene/ShapeRenderer.h>
//STD
#include <functional>
////////////////////////////////////////////////////////////
namespace nero
{
    class SceneRenderer : public Engine
    {
        public:
            typedef std::shared_ptr<SceneRenderer> Ptr;

                                SceneRenderer(const float& windowWidth = 800.f, const float& windowHeight = 600.f, const sf::String& title = "Nero Render Engine");
            virtual            ~SceneRenderer();

            void                setScene(Scene::Ptr scene);
            void                setRestartScene(std::function<void()> fn);
            void                setQuit(std::function<void()> fn);


        private: //Methods
            void                handleEvent();
            void                update(const sf::Time& timeStep);
            void                render();
            void                renderScene();
            void                renderSceneFront();

        private: //Attribute
            friend class                DevEngine;
            friend class                DevEngineUI;
            friend class                Scene;

            Camera::Ptr                 m_Camera;
            sf::View                    m_FrontView;
            Scene::Ptr                  m_Scene;
            Scene::Ptr                  m_RawScene;
            ShapeRenderer               m_ShapeRenderer;
            bool                        m_RestartScene;
            std::function<void()>       m_RestartSceneFn;
            std::function<void()>       m_QuitFn;

    };
}
#endif // SCENERENDERER_H
