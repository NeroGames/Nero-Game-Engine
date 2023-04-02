////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicsObject.h>
#include <Nero/core/cpp/model/PointMesh.h>
#include <Nero/core/cpp/scene/PhysicsUtility.h>
// BOX2D
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2Body.h>
// Std
#include <vector>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicsManager
    {
      public:
        PhysicsManager();
        virtual ~PhysicsManager();

        PhysicsObject::Ptr createObject(PointMesh::Ptr pointMesh, const PhysicsPoperty& physics);
        void               setPhysicsWorld(std::shared_ptr<b2World> physicsWorld);

      private:
        void setupBodyDef(PointMesh::Ptr pointMesh, const PhysicsPoperty& physics);
        void setupFixtureDef(const PhysicsPoperty& physics);
        void setupVertexTable(b2Vec2* table, PointMesh::Ptr pointMesh);
        void computePolygonBody(b2Body*              body,
                                b2FixtureDef*        fixtureDef,
                                std::vector<b2Vec2>* verticesVec,
                                int                  scale);

      private:
        std::shared_ptr<b2World> m_PhysicsWorld;
        b2BodyDef                m_BodyDef;
        b2FixtureDef             m_FixtureDef;
    };
} // namespace nero
#endif // PHYSICSMANAGER_H
