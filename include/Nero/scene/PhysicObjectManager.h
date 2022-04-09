////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICOBJECTMANAGER_H
#define PHYSICOBJECTMANAGER_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/PhysicObject.h>
#include <Nero/model/Mesh.h>
//BOX2D
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2Body.h>
//STD
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicObjectManager
    {
        public:
            typedef std::vector<PhysicObject>   ObjectTab;
            typedef std::vector<Mesh*>          MeshTab;

        public:
                                PhysicObjectManager();
            virtual            ~PhysicObjectManager();

            PhysicObject::Ptr   createObject(Mesh* mesh);
            void                destroyAllObject();
            void                setWorld(b2World* world);

        private:
            void                setupBodyDef(Mesh& mesh);
            void                setupFixtureDef(Mesh& mesh);
            void                setupVertexTab(b2Vec2* tab, Mesh& mesh);
            void                computePolygonBody(b2Body* pBody, b2FixtureDef* pFixtureDef, std::vector<b2Vec2>* pVerticesVec, int scale);

        private:
            b2World*            m_World;
            ObjectTab           m_ObjectTab;
            int                 m_ObjectCount;

            b2BodyDef           m_BodyDef;
            b2FixtureDef        m_FixtureDef;
    };
}
#endif // PHYSICOBJECTMANAGER_H
