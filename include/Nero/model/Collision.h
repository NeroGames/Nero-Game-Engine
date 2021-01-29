////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef COLLISION_H
#define COLLISION_H
///////////////////////////HEADERS//////////////////////////
//NEROCopyright (c) 2016-2021 Sanou A. K. Landry
#include <Nero/object/PhysicObject.h>
//BOX2D
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class Collision
    {
        public:
                                        Collision(b2Contact* contact, const b2Manifold* oldManifold, const b2ContactImpulse*   m_ContactImpulse);
                                       ~Collision();
            //Initialize Collision
            void                        setObjectA(PhysicObject::Ptr objectA);
            void                        setObjectB(PhysicObject::Ptr objectB);

            PhysicObject::Ptr           getObjectA();
            PhysicObject::Ptr           getObjectB();

            //Contact
            float                       getFriction();
            float                       getRestition();
            float                       getTangentSpeed();

            bool                        isEnabled();
            bool                        isTouching();

            void                        resetFriction();
            void                        resetRestitution();

            void                        setEnabled(bool flag);
            void                        setFriction(float friction);
            void                        setRestitution(float restitution);
            void                        setTangentSpeed(float speed);

            //Contact Manifold
            sf::Vector2f                getLocalNormal();
            sf::Vector2f                getLocalPoint();
            int                         getPointCount();
            float                       getNormalImpulse(int index);
            float                       getTangentImpulse(int index);

            //Contact world Manifold
            sf::Vector2f                getNormal();
            std::vector<sf::Vector2f>   getPoints();
            std::vector<float>          getSeparations();

            //Old Manifold
            sf::Vector2f                getOldLocalNormal();
            sf::Vector2f                getOldLocalPoint();
            int                         getOldPointCount();
            float                       getOldNormalImpulse(int index);
            float                       getOldTangentImpulse(int index);

            //Contact Impulse : PostSolve
            int                         getImpulseCount();
            std::vector<float>          getNormalImpulses();
            std::vector<float>          getTangentImpulses();
            //
            bool                        isColliding(sf::String catA);
            bool                        isColliding(sf::String catA, sf::String catB);
            bool                        isObjectColliding(sf::String nameA);
            bool                        isObjectColliding(sf::String nameA, sf::String nameB);

            PhysicObject::Ptr           getObject(sf::String indicator);

        private:
            b2Contact*                  m_Contact;
            b2WorldManifold*            m_WorldManifold;
            const b2Manifold*           m_OldManifold;
            const b2ContactImpulse*     m_ContactImpulse;
            PhysicObject::Ptr           m_ObjectA;
            PhysicObject::Ptr           m_ObjectB;
    };
}
#endif // COLLIION_H
