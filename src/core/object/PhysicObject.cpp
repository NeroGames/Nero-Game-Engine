////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/object/PhysicObject.h>
#include <Nero/core/utility/Utility.h>
//BOX2D
#include <Box2D/Dynamics/b2Fixture.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicObject::PhysicObject(b2Body* body):
         m_Body(body)
        ,m_Dead(false)
        ,m_Size(sf::Vector2f(0.f, 0.f))
    {
        setFirstType(Object::Physic_Object);
        setSecondType(Object::Physic_Object);
    }

    b2Body* PhysicObject::getBody()
    {
        return m_Body;
    }

    void PhysicObject::updateObject(sf::Time time_step)
    {
        setPosition(b2_to_sf(m_Body->GetPosition(), SCALE));
        setRotation(toDegree(m_Body->GetAngle()));
    }

    void PhysicObject::setUserData(void* userData)
    {
        m_Body->SetUserData(userData);
    }

    void PhysicObject::setCollisionRule(const Rule& rule)
    {
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            b2Filter filter;
            filter.categoryBits = rule.categoryBits;
            filter.maskBits = rule.maskBits;

            fixture->SetFilterData(filter);
        }
    }

    PhysicObject::Ptr PhysicObject::Cast(Object::Ptr object)
    {
        if(!object || object->getFirstType() != Object::Physic_Object)
            return nullptr;

        return  std::static_pointer_cast<PhysicObject>(object);
    }

    float PhysicObject::getAngle() const
    {
        return toDegree(m_Body->GetAngle());
    }

    float PhysicObject::getAngularDamping() const
    {
        return m_Body->GetAngularDamping();
    }

    float PhysicObject::getAngularVelocity() const
    {
        return toDegree(m_Body->GetAngularVelocity());
    }

    float PhysicObject::getGravityScale() const
    {
        return m_Body->GetGravityScale();
    }

    float PhysicObject::getLinearDamping() const
    {
        return m_Body->GetLinearDamping();
    }

    sf::Vector2f PhysicObject::getLinearVelocity() const
    {
        return b2_to_sf(m_Body->GetLinearVelocity(), SCALE);
    }

    sf::Vector2f PhysicObject::getLocalCenter() const
    {
        return b2_to_sf(m_Body->GetLocalCenter(), SCALE);
    }

    float PhysicObject::getMass() const
    {
        return m_Body->GetMass();
    }

    sf::Vector2f PhysicObject::getPosition() const
    {
        return b2_to_sf(m_Body->GetPosition(), SCALE);
    }

    sf::Vector2f PhysicObject::getWorldCenter() const
    {
        return b2_to_sf(m_Body->GetWorldCenter(), SCALE);
    }

    sf::Vector2f PhysicObject::getCenter() const
    {
        return getWorldCenter();
    }

    void PhysicObject::applyAngularImpulse(float impulse, bool wake)
    {
        m_Body->ApplyAngularImpulse(impulse, wake);
    }

    void PhysicObject::applyForce(const sf::Vector2f& force, const sf::Vector2f& origin, bool wake)
    {
        m_Body->ApplyForce(sf_to_b2(force, SCALE), sf_to_b2(origin, SCALE), wake);
    }

    void PhysicObject::applyForceToCenter(const sf::Vector2f& force, bool wake)
    {
        m_Body->ApplyForceToCenter(sf_to_b2(force, SCALE), wake);
    }

    void PhysicObject::applyLinearImpulse(const sf::Vector2f& impulse, const sf::Vector2f& origin, bool wake)
    {
        m_Body->ApplyLinearImpulse(sf_to_b2(impulse, SCALE), sf_to_b2(origin, SCALE), wake);
    }

    void PhysicObject::applyTorque(float torque, bool wake)
    {
        m_Body->ApplyTorque(torque, wake);
    }

    float PhysicObject::getInertia() const
    {
        return m_Body->GetInertia();
    }

    bool PhysicObject::isActive() const
    {
        return m_Body->IsActive();
    }

    bool PhysicObject::isAwake()  const
    {
        return m_Body->IsAwake();
    }

    bool PhysicObject::isBullet() const
    {
        return m_Body->IsBullet();
    }

    bool PhysicObject::isFixedRotation() const
    {
        return m_Body->IsFixedRotation();
    }

    bool PhysicObject::isSleepingAllowed() const
    {
        return m_Body->IsSleepingAllowed();
    }

    void PhysicObject::setActive(bool flag)
    {
        m_Body->SetActive(flag);
    }

    void PhysicObject::setAngularDamping(float angular_damping)
    {
        m_Body->SetAngularDamping(angular_damping);
    }

    void PhysicObject::setAngularVelocity(float angle)
    {
        m_Body->SetAngularVelocity(toRadian(angle));
    }

    void PhysicObject::setAwake(bool flag)
    {
        m_Body->SetAwake(flag);
    }

    void PhysicObject::setBullet(bool flag)
    {
        m_Body->SetBullet(flag);
    }

    void PhysicObject::setFixedRotation(bool flag)
    {
        m_Body->SetFixedRotation(flag);
    }

    void PhysicObject::setGravityScale(float scale)
    {
        m_Body->SetGravityScale(scale);
    }

    void PhysicObject::setLinearDamping(float linear_damping)
    {
        m_Body->SetLinearDamping(linear_damping);
    }

    void PhysicObject::setLinearVelocity(const sf::Vector2f& velocity)
    {
        m_Body->SetLinearVelocity(sf_to_b2(velocity, SCALE));
    }

    void PhysicObject::setSleepingAllowed(bool flag)
    {
        m_Body->SetSleepingAllowed(flag);
    }

    bool PhysicObject::isSensor() const
    {
        return m_Body->GetFixtureList()->IsSensor();
    }

    float PhysicObject::getDensity() const
    {
        return m_Body->GetFixtureList()->GetDensity();
    }

    float PhysicObject::getFriction() const
    {
        return m_Body->GetFixtureList()->GetFriction() * 10.f;
    }

    float PhysicObject::getRestitution() const
    {
        return m_Body->GetFixtureList()->GetRestitution() * 10.f;
    }

    void PhysicObject::setDensity(float density)
    {
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
           fixture->SetDensity(density);
        }
    }

    void PhysicObject::setFriction(float friction)
    {
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
           fixture->SetFriction(friction);
        }
    }

    void PhysicObject::setRestitution(float restitution)
    {
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
           fixture->SetRestitution(restitution);
        }
    }

    void PhysicObject::setSensor(bool flag)
    {
        for (b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
           fixture->SetSensor(flag);
        }
    }

    void PhysicObject::clearForce()
    {
       setLinearVelocity(sf::Vector2f(0.f, 0.f));
       setAngularVelocity(0.f);
    }

    void PhysicObject::setAngle(float angle)
    {
        bool was_fixedRotation = isFixedRotation();

        if(was_fixedRotation)
        {
            setFixedRotation(false);
            m_Body->SetTransform(m_Body->GetPosition(), toRadian(angle));
            setFixedRotation(true);
        }
        else
        {
            m_Body->SetTransform(m_Body->GetPosition(), toRadian(angle));
        }
    }

    sf::Vector2f PhysicObject::getAxis_x() const
    {
        b2Transform t = m_Body->GetTransform();
        t.p = m_Body->GetWorldCenter();

        b2Vec2 p1 = t.p;
        b2Vec2 p2 = p1  + t.q.GetXAxis();

        return unitVector(b2_to_sf(p2-p1));
    }

    sf::Vector2f PhysicObject::getAxis_y() const
    {
        b2Transform t = m_Body->GetTransform();
        t.p = m_Body->GetWorldCenter();

        b2Vec2 p1 = t.p;
        b2Vec2 p2 = p1  + t.q.GetYAxis();

        return unitVector(b2_to_sf(p2-p1));
    }

    void PhysicObject::clearVelocity()
    {
        setLinearVelocity(sf::Vector2f(0.f, 0.f));
    }

    void PhysicObject::clearHorizontalVelocity()
    {
        setLinearVelocity(sf::Vector2f(0.f, getLinearVelocity().y));
    }

    void PhysicObject::clearVerticalVelocity()
    {
        setLinearVelocity(sf::Vector2f(getLinearVelocity().x, 0.f));
    }

    void PhysicObject::clearAngularVelocity()
    {
       setAngularVelocity(0.f);
    }

     bool PhysicObject::isDead() const
     {
         return m_Dead;
     }

    void PhysicObject::setDead(bool flag)
    {
        m_Dead = flag;
    }

    void PhysicObject::setSize(const sf::Vector2f& size)
    {
        m_Size = size;
    }

    sf::Vector2f PhysicObject::getSize()
    {
        return m_Size;
    }

    sf::FloatRect PhysicObject::getGlobalBounds() const
    {
        sf::FloatRect bound;

        bound.width = m_Size.x;
        bound.height = m_Size.y;

        sf::Vector2f position = getWorldCenter();

        bound.left = position.x - m_Size.x / 2.f;
        bound.top = position.y - m_Size.y / 2.f;

        return bound;
    }
}
