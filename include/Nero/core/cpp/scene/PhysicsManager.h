////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicObject.h>
#include <Nero/core/cpp/model/PointMesh.h>
// BOX2D
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2Body.h>
// STD
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicsManager
    {
      public:
        PhysicsManager();
        virtual ~PhysicsManager();

        PhysicObject::Ptr createObject(PointMesh::Ptr pointMesh);
        void              setWorld(b2World* physicsWorld);

      private:
        void setupBodyDef(PointMesh::Ptr pointMesh);
        void setupFixtureDef(PointMesh::Ptr pointMesh);
        void setupVertexTable(b2Vec2* table, PointMesh::Ptr pointMesh);
        void computePolygonBody(b2Body*              body,
                                b2FixtureDef*        fixtureDef,
                                std::vector<b2Vec2>* verticesVec,
                                int                  scale);

      private:
        b2World*     m_PhysicsWorld;
        b2BodyDef    m_BodyDef;
        b2FixtureDef m_FixtureDef;
    };
} // namespace nero
#endif // PHYSICSMANAGER_H
