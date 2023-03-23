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

    void PhysicsMeshObject::setMeshType(PointMesh::Type type)
    {
        m_PointMesh->setMeshType(type);
    }

    nlohmann::json PhysicsMeshObject::toJson() const
    {
        nlohmann::json mesh_json;

        mesh_json         = Object::toJson();
        mesh_json["mesh"] = m_PointMesh->toJson();

        return mesh_json;
    }

    Object::Ptr PhysicsMeshObject::clone() const
    {
        auto meshObject = std::make_shared<PhysicsMeshObject>();
        Object::clone<PhysicsMeshObject::Ptr>(meshObject);
        meshObject->setMesh(m_PointMesh->clone());

        return meshObject;
    }

    Object::Ptr PhysicsMeshObject::clone(sf::Vector2f& position) const
    {
        PhysicsMeshObject::Ptr meshObject = Cast(clone());

        meshObject->setId(-1);
        meshObject->getMesh()->setMeshId(-1);
        meshObject->setPosition(position);

        return meshObject;
    }

    PhysicsMeshObject::Ptr PhysicsMeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return std::static_pointer_cast<PhysicsMeshObject>(object);
    }

} // namespace nero
