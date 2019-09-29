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

    }

    Scene::~Scene()
    {
        destroyScene();
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
}
