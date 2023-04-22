////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/core/cpp/scene/ContactListener.h>
// Box2d
#include <Box2D/Dynamics/Contacts/b2Contact.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    ContactListener::ContactListener(ObjectManager::Ptr objectManager)
        : m_ObjectManager(objectManager)
        , m_MaxContactPoint(2048)
        , m_PointCount(0)
    {
    }

    void ContactListener::registerCallback(const std::string&             name,
                                           std::function<void(Collision)> callback)
    {
        if(name == "beginContact")
        {
            m_BeginContactCallback = callback;
        }
        else if(name == "endContact")
        {
            m_EndContactCallback = callback;
        }
        else if(name == "preSolveContact")
        {
            m_PreSolveContactCallback = callback;
        }
        else if(name == "postSolveContact")
        {
            m_PostSolveContactCallback = callback;
        }
    }

    void ContactListener::BeginContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int       idA     = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int       idB     = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto      objectA = PhysicsObject::Cast(m_ObjectManager->findObject(idA));
        auto      objectB = PhysicsObject::Cast(m_ObjectManager->findObject(idB));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        m_BeginContactCallback(collision);
    }

    void ContactListener::EndContact(b2Contact* contact)
    {
        Collision collision(contact, nullptr, nullptr);

        int       idA     = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int       idB     = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto      objectA = PhysicsObject::Cast(m_ObjectManager->findObject(idA));
        auto      objectB = PhysicsObject::Cast(m_ObjectManager->findObject(idB));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        m_EndContactCallback(collision);
    }

    void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
    {
        const b2Manifold* manifold = contact->GetManifold();

        if(manifold->pointCount == 0)
        {
            return;
        }

        b2Fixture*   fixtureA = contact->GetFixtureA();
        b2Fixture*   fixtureB = contact->GetFixtureB();

        b2PointState state1[b2_maxManifoldPoints], state2[b2_maxManifoldPoints];
        b2GetPointStates(state1, state2, oldManifold, manifold);

        b2WorldManifold worldManifold;
        contact->GetWorldManifold(&worldManifold);

        for(int32 i = 0; i < manifold->pointCount && m_PointCount < m_MaxContactPoint; ++i)
        {
            ContactPoint* cp   = m_PointTable + m_PointCount;
            cp->fixtureA       = fixtureA;
            cp->fixtureB       = fixtureB;
            cp->position       = worldManifold.points[i];
            cp->normal         = worldManifold.normal;
            cp->state          = state2[i];
            cp->normalImpulse  = manifold->points[i].normalImpulse;
            cp->tangentImpulse = manifold->points[i].tangentImpulse;
            cp->separation     = worldManifold.separations[i];
            ++m_PointCount;
        }

        Collision collision(contact, oldManifold, nullptr);

        int       idA     = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int       idB     = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto      objectA = PhysicsObject::Cast(m_ObjectManager->findObject(idA));
        auto      objectB = PhysicsObject::Cast(m_ObjectManager->findObject(idB));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        m_PreSolveContactCallback(collision);
    }

    void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        Collision collision(contact, nullptr, impulse);

        int       idA     = (int)contact->GetFixtureA()->GetBody()->GetUserData();
        int       idB     = (int)contact->GetFixtureB()->GetBody()->GetUserData();

        auto      objectA = PhysicsObject::Cast(m_ObjectManager->findObject(idA));
        auto      objectB = PhysicsObject::Cast(m_ObjectManager->findObject(idB));

        if(!objectA || !objectB)
            return;

        if(objectA->isDead() || objectB->isDead())
            return;

        collision.setObjectA(objectA);
        collision.setObjectB(objectB);

        m_PostSolveContactCallback(collision);
    }

    unsigned int ContactListener::getContactPointCount() const
    {
        return m_PointCount;
    }

    ContactPoint* ContactListener::getContactPointTable()
    {
        return m_PointTable;
    }

    void ContactListener::resetContactPointCount()
    {
        m_PointCount = 0;
    }

} // namespace nero
