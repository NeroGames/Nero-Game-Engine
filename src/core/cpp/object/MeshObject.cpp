////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
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
        , m_Mesh()
    {
        setFirstType(Object::Mesh_Object);
        setSecondType(Object::Mesh_Object);
    }

    MeshObject::~MeshObject()
    {
    }

    void MeshObject::setMesh(const Mesh& mesh)
    {
        m_Mesh = mesh;
    }

    Mesh* MeshObject::getMesh()
    {
        return &m_Mesh;
    }

    void MeshObject::drawObject(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(m_Mesh, states);
    }

    sf::FloatRect MeshObject::getGlobalBounds() const
    {
        return m_Mesh.getGlobalBounds();
    }

    void MeshObject::updateObject(sf::Time time_step)
    {
        sf::Vector2f position = getPosition();
        sf::Vector2f scale    = getScale();
        float        rotation = getRotation();
        for(Object* parent = getParent(); parent != nullptr; parent = parent->getParent())
        {
            position += parent->getPosition();
            scale.x *= parent->getScale().x;
            scale.y *= parent->getScale().y;
            rotation += parent->getRotation();
        }

        m_Mesh.updateMesh(position, scale, rotation);
    }

    void MeshObject::setMeshType(Mesh::Type type)
    {
        // m_Mesh.setType(type);
        // m_Mesh.updateColor();
    }

    void MeshObject::setMeshFixedRotation(bool flag)
    {
        // m_Mesh.setFixedRotation(flag);
    }

    void MeshObject::setMeshSensor(bool flag)
    {
        // m_Mesh.setIsSensor(flag);
    }

    void MeshObject::setMeshAllowSleep(bool flag)
    {
        // m_Mesh.setAllowSleep(flag);
    }

    void MeshObject::setMeshDensity(const float& density)
    {
        // m_Mesh.setDensity(density);
    }

    void MeshObject::setMeshFriction(const float& friction)
    {
        // m_Mesh.setFriction(friction);
    }

    void MeshObject::setMeshRestitution(const float& restitution)
    {
        // m_Mesh.setRestitution(restitution);
    }

    void MeshObject::setMeshGravityScale(const float& gravityScale)
    {
        // m_Mesh.setGravityScale(gravityScale);
    }

    bool MeshObject::getMeshFixedRotation() const
    {
        // return m_Mesh.getFixedRotation();
    }

    bool MeshObject::getMeshSensor() const
    {
        // return m_Mesh.getIsSensor();
    }

    bool MeshObject::getMeshAllowSleep() const
    {
        // return m_Mesh.getAllowSleep();
    }

    float MeshObject::getMeshDensity() const
    {
        // return m_Mesh.getDensity();
    }

    float MeshObject::getMeshFriction() const
    {
        // return m_Mesh.getFriction();
    }

    float MeshObject::getMeshRestitution() const
    {
        // return m_Mesh.getRestitution();
    }

    float MeshObject::getMeshGravityScale() const
    {
        // return m_Mesh.getGravityScale();
    }

    nlohmann::json MeshObject::toJson() const
    {
        nlohmann::json mesh_json;

        mesh_json = Object::toJson();
        // mesh_json["mesh"] = m_Mesh.toJson();

        return mesh_json;
    }

    Object::Ptr MeshObject::clone() const
    {
        MeshObject::Ptr mesh_object(new MeshObject());
        Object::clone<MeshObject::Ptr>(mesh_object);

        Mesh mesh = m_Mesh;
        mesh_object->setMesh(mesh);

        return mesh_object;
    }

    Object::Ptr MeshObject::clone(sf::Vector2f& position) const
    {
        MeshObject::Ptr mesh_object = Cast(clone());

        mesh_object->setId(-1);
        mesh_object->getMesh()->setMeshId(-1);

        return mesh_object;
    }

    MeshObject::Ptr MeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return std::static_pointer_cast<MeshObject>(object);
    }

} // namespace nero
