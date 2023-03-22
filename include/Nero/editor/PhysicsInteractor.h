////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PHYSICSINTERACTOR_H
#define PHYSICSINTERACTOR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/scene/ShapeRenderer.h>
// SFML
#include <SFML/System/Time.hpp>
// Box2d
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Collision/b2Collision.h>
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Dynamics/b2TimeStep.h>
#include <Box2D/Dynamics/b2World.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    // Forward declaration
    class PhysicsInteractor;

    class DestructionListener : public b2DestructionListener
    {
      public:
        void                               SayGoodbye(b2Fixture* fixture) override;
        void                               SayGoodbye(b2Joint* joint) override;

        std::shared_ptr<PhysicsInteractor> physicsInteractor;
    };

    class QueryCallback : public b2QueryCallback
    {
      public:
        QueryCallback(const b2Vec2& point);
        ~QueryCallback() override;

        bool       ReportFixture(b2Fixture* fixture) override;

        b2Vec2     m_Point;
        b2Fixture* m_Fixture;
    };

    class PhysicsInteractor
    {
      public:
        using Ptr = std::shared_ptr<PhysicsInteractor>;

      public:
        PhysicsInteractor();

        void initialize(std::shared_ptr<b2World> physicsWorld, ShapeRenderer::Ptr shapeRenderer);
        void update(const sf::Time& timeStep);
        void renderDebugData();

        // Mouse
        void shiftMouseDown(const b2Vec2& position);
        void mouseDown(const b2Vec2& position);
        void mouseUp(const b2Vec2& position);
        void mouseMove(const b2Vec2& position);
        // Bomb
        void launchBomb();
        void launchBomb(const b2Vec2& position, const b2Vec2& velocity);
        void spawnBomb(const b2Vec2& worldPosition);
        void completeBombSpawn(const b2Vec2& position);
        void destroyBomb();
        // Joint
        void jointDestroyed(b2Joint* joint);

      private:
        friend class DestructionListener;
        friend class BoundaryListener;
        friend class ContactListener;

        DestructionListener      m_DestructionListener;
        std::string              m_Message;
        std::string              m_StatMessage;
        std::string              m_ProfileMessage;
        b2Body*                  m_Bomb;
        b2Vec2                   m_BombSpawnPoint;
        bool                     m_BombSpawning;
        b2Body*                  m_GroundBody;
        b2AABB                   m_WorldAABB;
        b2MouseJoint*            m_MouseJoint;
        b2Vec2                   m_MouseWorld;
        int32                    m_StepCount;
        b2Profile                m_MaxProfile;
        b2Profile                m_TotalProfile;
        std::shared_ptr<b2World> m_PhysicsWorld;
        ShapeRenderer::Ptr       m_ShapeRenderer;
    };
} // namespace nero
#endif // PHYSICSINTERACTOR_H
