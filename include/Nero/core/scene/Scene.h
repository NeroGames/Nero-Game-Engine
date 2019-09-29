////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCENE_H
#define SCENE_H
///////////////////////////HEADERS///////////////////////////
//NERO
#include <Nero/core/object/GameObject.h>
//SFML
//BOX2D
#include <Box2D/Dynamics/b2World.h>
//STD
#include <memory>
#include <vector>
#include <boost/config.hpp>

/////////////////////////////////////////////////////////////
namespace nero
{
    class  Scene
    {
        public: //Types Definiton
            typedef std::shared_ptr<Scene> Ptr;
            typedef std::vector<GameObject::Ptr> WorldChunkTab;

        public: //Utility Class
            struct Context
            {
                Context();
            };


        public: //Constructor
                                            Scene(Context context);
              virtual                             ~Scene();

        private:
            void                            destroyScene();

        private:
            //Base Attributs
            Context                             m_SceneContext;
            b2World*                            m_PhysicWorld;
            GameObject                          m_GameWorld;
            //Containers
            std::vector<GameObject::Ptr>        m_WorldChunkTable;

        public: //test
            virtual std::string                         getName();

    };
}



#endif // SCENE_H
