////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#include <Nero/core/scene/Scene.h>

namespace nero
{
    Scene::Context::Context()
    {

    }

    Scene::Scene(Context context)
    {
        m_RenderTexture.create(800.f, 600.f);

    }

    Scene::~Scene()
    {
        destroyScene();
    }

    void Scene::handleEvent(const sf::Event& event)
    {

    }

    void Scene::update(const sf::Time& timeStep)
    {

    }

    void    Scene::renderScene()
    {
        m_RenderTexture.clear(sf::Color::Black);

        render();

        m_RenderTexture.display();
    }


    void Scene::render()
    {

    }


    void Scene::destroyScene()
    {

        //Destroy the Physic World
        //if(m_PhysicWorld)
        //{
            //delete(m_PhysicWorld);
            //m_PhysicWorld = nullptr;
        //}

    }

    std::string Scene::getName()
    {
        return  "Load Scene from DLL work";
    }

    sf::RenderTexture& Scene::getRenderTexture()
    {
        return m_RenderTexture;
    }

}
