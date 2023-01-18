////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICOBJECTMANAGER_H
#define PHYSICOBJECTMANAGER_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/PhysicObject.h>
#include <Nero/core/cpp/model/Mesh.h>
// BOX2D
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2Body.h>
// STD
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicObjectManager {
        /*public:
                        PhysicObjectManager();
    virtual            ~PhysicObjectManager();

    PhysicObject::Ptr   createObject(Mesh* mesh);
    void                setWorld(b2World* world);

private:
    void                setupBodyDef(Mesh& mesh);
    void                setupFixtureDef(Mesh& mesh);
    void                setupVertexTab(b2Vec2* tab, Mesh& mesh);
    void                computePolygonBody(b2Body* pBody, b2FixtureDef* pFixtureDef, std::vector<b2Vec2>* pVerticesVec, int scale);

private:
    b2World*            m_World;

    b2BodyDef           m_BodyDef;
                b2FixtureDef        m_FixtureDef;*/
    };
} // namespace nero
#endif // PHYSICOBJECTMANAGER_H
