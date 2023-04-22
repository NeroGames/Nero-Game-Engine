////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef CONTACTLISTENER_H
#define CONTACTLISTENER_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/ObjectManager.h>
#include <Nero/core/cpp/model/Collision.h>
// Box2d
// Std
#include <functional>
/////////////////////////////////////////////////////////////
namespace nero
{
    struct ContactPoint
    {
        b2Fixture*   fixtureA;
        b2Fixture*   fixtureB;
        b2Vec2       normal;
        b2Vec2       position;
        b2PointState state;
        float32      normalImpulse;
        float32      tangentImpulse;
        float32      separation;
    };

    class ContactListener : public b2ContactListener
    {
      public:
        using Ptr = std::shared_ptr<ContactListener>;

      public:
        ContactListener(ObjectManager::Ptr objectManager);

        virtual void BeginContact(b2Contact* contact) override;
        virtual void EndContact(b2Contact* contact) override;
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
        void registerCallback(const std::string& name, std::function<void(Collision)> callback);

        unsigned int  getContactPointCount() const;
        ContactPoint* getContactPointTable();
        void          resetContactPointCount();

      private:
        ObjectManager::Ptr             m_ObjectManager;
        unsigned int                   m_MaxContactPoint;
        ContactPoint                   m_PointTable[2048];
        unsigned int                   m_PointCount;

        std::function<void(Collision)> m_BeginContactCallback;
        std::function<void(Collision)> m_EndContactCallback;
        std::function<void(Collision)> m_PreSolveContactCallback;
        std::function<void(Collision)> m_PostSolveContactCallback;
    };

} // namespace nero
#endif // CONTACTLISTENER_H
