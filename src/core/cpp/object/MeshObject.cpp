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
    MeshObject::MeshObject()
        : Object()
        , m_Mesh(nullptr)
    {
        setFirstType(Object::Mesh_Object);
        setSecondType(Object::Mesh_Object);
    }

    MeshObject::~MeshObject()
    {
    }

    void MeshObject::setMesh(const PointMesh::Ptr& mesh)
    {
        m_Mesh = mesh;
    }

    PointMesh::Ptr MeshObject::getMesh()
    {
        return m_Mesh;
    }

    void MeshObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(*m_Mesh, states);
    }

    sf::FloatRect MeshObject::getGlobalBounds() const
    {
        return m_Mesh->getGlobalBounds();
    }

    void MeshObject::setMeshType(PointMesh::Type type)
    {
        m_Mesh->setMeshType(type);
    }

    nlohmann::json MeshObject::toJson() const
    {
        nlohmann::json mesh_json;

        mesh_json         = Object::toJson();
        mesh_json["mesh"] = m_Mesh->toJson();

        return mesh_json;
    }

    Object::Ptr MeshObject::clone() const
    {
        MeshObject::Ptr mesh_object(new MeshObject());
        Object::clone<MeshObject::Ptr>(mesh_object);

        PointMesh::Ptr mesh = std::make_shared<PointMesh>();
        // TODO copy meshs properties
        mesh_object->setMesh(mesh);

        return mesh_object;
    }

    Object::Ptr MeshObject::clone(sf::Vector2f& position) const
    {
        MeshObject::Ptr meshObject = Cast(clone());

        meshObject->setId(-1);
        meshObject->getMesh()->setMeshId(-1);

        return meshObject;
    }

    MeshObject::Ptr MeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return std::static_pointer_cast<MeshObject>(object);
    }

} // namespace nero
