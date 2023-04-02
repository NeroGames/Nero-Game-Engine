////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicsMeshObject.h>
// EASYLOG
// #include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicsMeshObject::PhysicsMeshObject()
        : Object()
        , m_PointMesh(nullptr)
        , m_PhysicsPoperty{false, false, true, 1.f, 0.1f, 0.1f, 3.f}
    {
        setFirstType(Object::Mesh_Object);
        setSecondType(Object::Mesh_Object);
    }

    PhysicsMeshObject::~PhysicsMeshObject()
    {
    }

    void PhysicsMeshObject::setMesh(const PointMesh::Ptr& pointMesh)
    {
        m_PointMesh = pointMesh;
    }

    PointMesh::Ptr PhysicsMeshObject::getMesh()
    {
        return m_PointMesh;
    }

    void PhysicsMeshObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(*m_PointMesh, states);
    }

    void PhysicsMeshObject::updateObject(sf::Time)
    {
        sf::Vector2f position = getPosition();
        sf::Vector2f scale    = getScale();
        float        rotation = getRotation();
        for(Object* parent = getParent(); parent != nullptr; parent = parent->getParent())
        {
            position += parent->getPosition();
            scale.x  *= parent->getScale().x;
            scale.y  *= parent->getScale().y;
            rotation += parent->getRotation();
        }

        m_PointMesh->transform(position, scale, rotation);
    }

    sf::FloatRect PhysicsMeshObject::getGlobalBounds() const
    {
        return m_PointMesh->getGlobalBounds();
    }

    nlohmann::json PhysicsMeshObject::toJson() const
    {
        nlohmann::json meshJson;

        meshJson             = Object::toJson();
        meshJson["mesh"]     = m_PointMesh->toJson();
        meshJson["rotation"] = getRotation();
        meshJson["position"] = {
            {"x", getPosition().x},
            {"y", getPosition().y}
        };
        meshJson["scale"] = {
            {"x", getScale().x},
            {"y", getScale().y}
        };

        meshJson["density"]        = m_PhysicsPoperty.density;
        meshJson["friction"]       = m_PhysicsPoperty.friction;
        meshJson["restitution"]    = m_PhysicsPoperty.restitution;
        meshJson["fixed_rotation"] = m_PhysicsPoperty.fixedRotation;
        meshJson["sensor"]         = m_PhysicsPoperty.sensor;
        meshJson["allow_sleep"]    = m_PhysicsPoperty.allowSleep;
        meshJson["gravity_scale"]  = m_PhysicsPoperty.gravityScale;

        return meshJson;
    }

    Object::Ptr PhysicsMeshObject::clone() const
    {
        auto meshObject = std::make_shared<PhysicsMeshObject>();
        Object::clone<PhysicsMeshObject::Ptr>(meshObject);
        meshObject->setMesh(m_PointMesh->clone());
        meshObject->setPosition(getPosition());
        meshObject->setScale(getScale());
        meshObject->setRotation(getRotation());
        return meshObject;
    }

    Object::Ptr PhysicsMeshObject::clone(sf::Vector2f& position) const
    {
        PhysicsMeshObject::Ptr meshObject = Cast(clone());

        meshObject->setId(-1);
        meshObject->getMesh()->setMeshId(-1);
        meshObject->move(position);
        return meshObject;
    }

    PhysicsMeshObject::Ptr PhysicsMeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return std::static_pointer_cast<PhysicsMeshObject>(object);
    }

    void PhysicsMeshObject::setProperty(const std::string& name, const bool value)
    {
        if(name == "fixed_rotation")
            m_PhysicsPoperty.fixedRotation = value;
        else if(name == "sensor")
            m_PhysicsPoperty.sensor = value;
        else if(name == "allow_sleep")
            m_PhysicsPoperty.allowSleep = value;
    }

    bool PhysicsMeshObject::getBoolProperty(const std::string& name)
    {
        if(name == "fixed_rotation")
            return m_PhysicsPoperty.fixedRotation;
        else if(name == "sensor")
            return m_PhysicsPoperty.sensor;
        else if(name == "allow_sleep")
            return m_PhysicsPoperty.allowSleep;

        throw std::exception();
    }

    void PhysicsMeshObject::setProperty(const std::string& name, const float& value)
    {
        if(name == "density")
            m_PhysicsPoperty.density = value;
        else if(name == "friction")
            m_PhysicsPoperty.friction = value;
        else if(name == "restitution")
            m_PhysicsPoperty.restitution = value;
        else if(name == "gravity_scale")
            m_PhysicsPoperty.gravityScale = value;
    }

    float PhysicsMeshObject::getFloatProperty(const std::string& name)
    {
        if(name == "density")
            return m_PhysicsPoperty.density;
        else if(name == "friction")
            return m_PhysicsPoperty.friction;
        else if(name == "restitution")
            return m_PhysicsPoperty.restitution;
        else if(name == "gravity_scale")
            return m_PhysicsPoperty.gravityScale;

        throw std::exception();
    }

    const PhysicsPoperty PhysicsMeshObject::getPhysicsPoperty() const
    {
        return m_PhysicsPoperty;
    }

} // namespace nero
