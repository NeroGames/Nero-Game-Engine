////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/MeshObject.h>
// EASYLOG
// #include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicalMeshObject::PhysicalMeshObject()
        : Object()
        , m_PointMesh(nullptr)
    {
        setFirstType(Object::Mesh_Object);
        setSecondType(Object::Mesh_Object);
    }

    PhysicalMeshObject::~PhysicalMeshObject()
    {
    }

    void PhysicalMeshObject::setMesh(const PointMesh::Ptr& pointMesh)
    {
        m_PointMesh = pointMesh;
    }

    PointMesh::Ptr PhysicalMeshObject::getMesh()
    {
        return m_PointMesh;
    }

    void PhysicalMeshObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(*m_PointMesh, states);
    }

    void PhysicalMeshObject::updateObject(sf::Time)
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

    sf::FloatRect PhysicalMeshObject::getGlobalBounds() const
    {
        return m_PointMesh->getGlobalBounds();
    }

    void PhysicalMeshObject::setMeshType(PointMesh::Type type)
    {
        m_PointMesh->setMeshType(type);
    }

    nlohmann::json PhysicalMeshObject::toJson() const
    {
        nlohmann::json mesh_json;

        mesh_json         = Object::toJson();
        mesh_json["mesh"] = m_PointMesh->toJson();

        return mesh_json;
    }

    Object::Ptr PhysicalMeshObject::clone() const
    {
        auto meshObject = std::make_shared<PhysicalMeshObject>();
        Object::clone<PhysicalMeshObject::Ptr>(meshObject);

        PointMesh::Ptr mesh = std::make_shared<PointMesh>();
        mesh->setMeshShape(m_PointMesh->getMeshShape());
        mesh->setMeshType(m_PointMesh->getMeshType());
        for(const auto& point : m_PointMesh->getPointTable())
        {
            mesh->addVertex(point);
        }
        meshObject->setMesh(mesh);

        return meshObject;
    }

    Object::Ptr PhysicalMeshObject::clone(sf::Vector2f& position) const
    {
        PhysicalMeshObject::Ptr meshObject = Cast(clone());

        meshObject->setId(-1);
        meshObject->getMesh()->setMeshId(-1);
        meshObject->setPosition(position);

        return meshObject;
    }

    PhysicalMeshObject::Ptr PhysicalMeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return std::static_pointer_cast<PhysicalMeshObject>(object);
    }

} // namespace nero
