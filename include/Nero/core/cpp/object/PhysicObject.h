////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/Object.h>
// BOX2D
#include <Box2D/Dynamics/b2Body.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicObject : public Object {
      public:
        typedef std::shared_ptr<PhysicObject> Ptr;
        static Ptr                            Cast(Object::Ptr object);

        struct Rule
        {
            int categoryBits;
            int maskBits;
        };

      public:
        PhysicObject(b2Body* body);
        virtual ~PhysicObject();

        b2Body*               getBody();
        void                  setUserData(void* userData);
        void                  setCollisionRule(const Rule& rule);

        // Body related methods
        float                 getAngle() const;
        float                 getAngularDamping() const;
        float                 getAngularVelocity() const;
        float                 getGravityScale() const;
        float                 getLinearDamping() const;
        float                 getMass() const;
        float                 getInertia() const;
        sf::Vector2f          getLinearVelocity() const;
        sf::Vector2f          getLocalCenter() const;
        sf::Vector2f          getPosition() const;
        sf::Vector2f          getWorldCenter() const;
        sf::Vector2f          getCenter() const;
        bool                  isActive() const;
        bool                  isAwake() const;
        bool                  isBullet() const;
        bool                  isFixedRotation() const;
        bool                  isSleepingAllowed() const;

        void                  applyAngularImpulse(float impulse, bool wake);
        void                  applyForce(const sf::Vector2f& force, const sf::Vector2f& origin, bool wake);
        void                  applyForceToCenter(const sf::Vector2f& force, bool wake);
        void                  applyLinearImpulse(const sf::Vector2f& impulse, const sf::Vector2f& origin, bool wake);
        void                  applyTorque(float torque, bool wake);

        void                  setActive(bool flag);
        void                  setAngularDamping(float angular_damping);
        void                  setAngularVelocity(float angle);
        void                  setAwake(bool flag);
        void                  setBullet(bool flag);
        void                  setFixedRotation(bool flag);
        void                  setGravityScale(float scale);
        void                  setLinearDamping(float linear_damping);
        void                  setLinearVelocity(const sf::Vector2f& velocity);
        void                  setSleepingAllowed(bool flag);

        bool                  isSensor() const;
        float                 getDensity() const;
        float                 getFriction() const;
        float                 getRestitution() const;
        void                  setDensity(float density);
        void                  setFriction(float friction);
        void                  setRestitution(float restitution);
        void                  setSensor(bool flag);

        void                  clearForce();
        void                  clearVelocity();
        void                  clearHorizontalVelocity();
        void                  clearVerticalVelocity();
        void                  clearAngularVelocity();
        void                  setAngle(float angle);

        sf::Vector2f          getAxis_x() const;
        sf::Vector2f          getAxis_y() const;

        bool                  isDead() const;
        void                  setDead(bool flag);

        void                  setSize(const sf::Vector2f& size);
        sf::Vector2f          getSize();
        virtual sf::FloatRect getGlobalBounds() const;

      private:
        void updateObject(sf::Time time_step);

      protected:
        b2Body*      m_Body;
        bool         m_Dead;
        sf::Vector2f m_Size;
    };
} // namespace nero
#endif // PHYSICOBJECT_H
