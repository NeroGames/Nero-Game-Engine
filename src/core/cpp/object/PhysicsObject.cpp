////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicsObject.h>
#include <Nero/core/cpp/utility/Utility.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
// BOX2D
#include <Box2D/Dynamics/b2Fixture.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicsObject::PhysicsObject(b2Body* body)
        : m_Body(body)
        , m_Dead(false)
        , m_Size(sf::Vector2f(0.f, 0.f))
    {
        setFirstType(Object::Physic_Object);
        setSecondType(Object::Physic_Object);
    }

    PhysicsObject::~PhysicsObject()
    {
        m_Body = nullptr;
    }

    b2Body* PhysicsObject::getBody()
    {
        return m_Body;
    }

    void PhysicsObject::updateObject(sf::Time time_step)
    {
        setPosition(graphics::b2_to_sf(m_Body->GetPosition(), EngineConstant.SCALE));
        setRotation(math::toDegree(m_Body->GetAngle()));
    }

    void PhysicsObject::setUserData(void* userData)
    {
        m_Body->SetUserData(userData);
    }

    void PhysicsObject::setCollisionRule(const Rule& rule)
    {
        for(b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            b2Filter filter;
            filter.categoryBits = rule.categoryBits;
            filter.maskBits     = rule.maskBits;

            fixture->SetFilterData(filter);
        }
    }

    PhysicsObject::Ptr PhysicsObject::Cast(Object::Ptr object)
    {
        if(!object || object->getFirstType() != Object::Physic_Object)
            return nullptr;

        return std::static_pointer_cast<PhysicsObject>(object);
    }

    float PhysicsObject::getAngle() const
    {
        return math::toDegree(m_Body->GetAngle());
    }

    float PhysicsObject::getAngularDamping() const
    {
        return m_Body->GetAngularDamping();
    }

    float PhysicsObject::getAngularVelocity() const
    {
        return math::toDegree(m_Body->GetAngularVelocity());
    }

    float PhysicsObject::getGravityScale() const
    {
        return m_Body->GetGravityScale();
    }

    float PhysicsObject::getLinearDamping() const
    {
        return m_Body->GetLinearDamping();
    }

    sf::Vector2f PhysicsObject::getLinearVelocity() const
    {
        return graphics::b2_to_sf(m_Body->GetLinearVelocity(), EngineConstant.SCALE);
    }

    sf::Vector2f PhysicsObject::getLocalCenter() const
    {
        return graphics::b2_to_sf(m_Body->GetLocalCenter(), EngineConstant.SCALE);
    }

    float PhysicsObject::getMass() const
    {
        return m_Body->GetMass();
    }

    sf::Vector2f PhysicsObject::getPosition() const
    {
        return graphics::b2_to_sf(m_Body->GetPosition(), EngineConstant.SCALE);
    }

    sf::Vector2f PhysicsObject::getWorldCenter() const
    {
        return graphics::b2_to_sf(m_Body->GetWorldCenter(), EngineConstant.SCALE);
    }

    sf::Vector2f PhysicsObject::getCenter() const
    {
        return getWorldCenter();
    }

    void PhysicsObject::applyAngularImpulse(float impulse, bool wake)
    {
        m_Body->ApplyAngularImpulse(impulse, wake);
    }

    void PhysicsObject::applyForce(const sf::Vector2f& force, const sf::Vector2f& origin, bool wake)
    {
        m_Body->ApplyForce(graphics::sf_to_b2(force, EngineConstant.SCALE),
                           graphics::sf_to_b2(origin, EngineConstant.SCALE),
                           wake);
    }

    void PhysicsObject::applyForceToCenter(const sf::Vector2f& force, bool wake)
    {
        m_Body->ApplyForceToCenter(graphics::sf_to_b2(force, EngineConstant.SCALE), wake);
    }

    void PhysicsObject::applyLinearImpulse(const sf::Vector2f& impulse,
                                           const sf::Vector2f& origin,
                                           bool                wake)
    {
        m_Body->ApplyLinearImpulse(graphics::sf_to_b2(impulse, EngineConstant.SCALE),
                                   graphics::sf_to_b2(origin, EngineConstant.SCALE),
                                   wake);
    }

    void PhysicsObject::applyTorque(float torque, bool wake)
    {
        m_Body->ApplyTorque(torque, wake);
    }

    float PhysicsObject::getInertia() const
    {
        return m_Body->GetInertia();
    }

    bool PhysicsObject::isActive() const
    {
        return m_Body->IsActive();
    }

    bool PhysicsObject::isAwake() const
    {
        return m_Body->IsAwake();
    }

    bool PhysicsObject::isBullet() const
    {
        return m_Body->IsBullet();
    }

    bool PhysicsObject::isFixedRotation() const
    {
        return m_Body->IsFixedRotation();
    }

    bool PhysicsObject::isSleepingAllowed() const
    {
        return m_Body->IsSleepingAllowed();
    }

    void PhysicsObject::setActive(bool flag)
    {
        m_Body->SetActive(flag);
    }

    void PhysicsObject::setAngularDamping(float angular_damping)
    {
        m_Body->SetAngularDamping(angular_damping);
    }

    void PhysicsObject::setAngularVelocity(float angle)
    {
        m_Body->SetAngularVelocity(math::toRadian(angle));
    }

    void PhysicsObject::setAwake(bool flag)
    {
        m_Body->SetAwake(flag);
    }

    void PhysicsObject::setBullet(bool flag)
    {
        m_Body->SetBullet(flag);
    }

    void PhysicsObject::setFixedRotation(bool flag)
    {
        m_Body->SetFixedRotation(flag);
    }

    void PhysicsObject::setGravityScale(float scale)
    {
        m_Body->SetGravityScale(scale);
    }

    void PhysicsObject::setLinearDamping(float linear_damping)
    {
        m_Body->SetLinearDamping(linear_damping);
    }

    void PhysicsObject::setLinearVelocity(const sf::Vector2f& velocity)
    {
        m_Body->SetLinearVelocity(graphics::sf_to_b2(velocity, EngineConstant.SCALE));
    }

    void PhysicsObject::setSleepingAllowed(bool flag)
    {
        m_Body->SetSleepingAllowed(flag);
    }

    bool PhysicsObject::isSensor() const
    {
        return m_Body->GetFixtureList()->IsSensor();
    }

    float PhysicsObject::getDensity() const
    {
        return m_Body->GetFixtureList()->GetDensity();
    }

    float PhysicsObject::getFriction() const
    {
        return m_Body->GetFixtureList()->GetFriction() * 10.f;
    }

    float PhysicsObject::getRestitution() const
    {
        return m_Body->GetFixtureList()->GetRestitution() * 10.f;
    }

    void PhysicsObject::setDensity(float density)
    {
        for(b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            fixture->SetDensity(density);
        }
    }

    void PhysicsObject::setFriction(float friction)
    {
        for(b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            fixture->SetFriction(friction);
        }
    }

    void PhysicsObject::setRestitution(float restitution)
    {
        for(b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            fixture->SetRestitution(restitution);
        }
    }

    void PhysicsObject::setSensor(bool flag)
    {
        for(b2Fixture* fixture = m_Body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            fixture->SetSensor(flag);
        }
    }

    void PhysicsObject::clearForce()
    {
        setLinearVelocity(sf::Vector2f(0.f, 0.f));
        setAngularVelocity(0.f);
    }

    void PhysicsObject::setAngle(float angle)
    {
        bool was_fixedRotation = isFixedRotation();

        if(was_fixedRotation)
        {
            setFixedRotation(false);
            m_Body->SetTransform(m_Body->GetPosition(), math::toRadian(angle));
            setFixedRotation(true);
        }
        else
        {
            m_Body->SetTransform(m_Body->GetPosition(), math::toRadian(angle));
        }
    }

    sf::Vector2f PhysicsObject::getAxis_x() const
    {
        b2Transform t = m_Body->GetTransform();
        t.p           = m_Body->GetWorldCenter();

        b2Vec2 p1     = t.p;
        b2Vec2 p2     = p1 + t.q.GetXAxis();

        return math::unitVector(graphics::b2_to_sf(p2 - p1));
    }

    sf::Vector2f PhysicsObject::getAxis_y() const
    {
        b2Transform t = m_Body->GetTransform();
        t.p           = m_Body->GetWorldCenter();

        b2Vec2 p1     = t.p;
        b2Vec2 p2     = p1 + t.q.GetYAxis();

        return math::unitVector(graphics::b2_to_sf(p2 - p1));
    }

    void PhysicsObject::clearVelocity()
    {
        setLinearVelocity(sf::Vector2f(0.f, 0.f));
    }

    void PhysicsObject::clearHorizontalVelocity()
    {
        setLinearVelocity(sf::Vector2f(0.f, getLinearVelocity().y));
    }

    void PhysicsObject::clearVerticalVelocity()
    {
        setLinearVelocity(sf::Vector2f(getLinearVelocity().x, 0.f));
    }

    void PhysicsObject::clearAngularVelocity()
    {
        setAngularVelocity(0.f);
    }

    bool PhysicsObject::isDead() const
    {
        return m_Dead;
    }

    void PhysicsObject::setDead(bool flag)
    {
        m_Dead = flag;
    }

    void PhysicsObject::setSize(const sf::Vector2f& size)
    {
        m_Size = size;
    }

    sf::Vector2f PhysicsObject::getSize()
    {
        return m_Size;
    }

    sf::FloatRect PhysicsObject::getGlobalBounds() const
    {
        sf::FloatRect bound;

        bound.width           = m_Size.x;
        bound.height          = m_Size.y;

        sf::Vector2f position = getWorldCenter();

        bound.left            = position.x - m_Size.x / 2.f;
        bound.top             = position.y - m_Size.y / 2.f;

        return bound;
    }
} // namespace nero
