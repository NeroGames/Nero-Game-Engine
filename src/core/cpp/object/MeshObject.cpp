////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/MeshObject.h>
//EASYLOG
//#include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    MeshObject::MeshObject():
        Object(),
        m_Mesh(),
        m_ParentLastPosition(sf::Vector2f(0.f,0.f)),
        m_ParentLastScale(sf::Vector2f(1.f, 1.f)),
        m_ParentLastRotation(0.f)
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
		//m_Mesh.update(states.transform);
        target.draw(m_Mesh, states);
    }

    sf::FloatRect MeshObject::getGlobalBounds() const
	{
		return  m_Mesh.getGlobalBounds();

		/* TODO remove
		if(isSelectable())
			return  m_Mesh.getGlobalBounds();
        else
			return sf::FloatRect();*/
    }

    void MeshObject::updateObject(sf::Time time_step)
    {
		//panning
		/*auto pos_diff = m_Parent->getPosition() - m_ParentLastPosition;
        auto pos = getPosition();
        setPosition(0.f, 0.f);

        m_Mesh.move(pos + pos_diff);

		m_ParentLastPosition = m_Parent->getPosition();

		//rotation
        float rot_diff = m_Parent->getRotation() - m_ParentLastRotation;
        float rot = getRotation();
        setRotation(0.f);

        m_Mesh.rotate(rot + rot_diff);

		m_ParentLastRotation = m_Parent->getRotation();

		//scaling
        auto scale_diff = m_Parent->getScale() - m_ParentLastScale;
        auto scale = getScale();
        setScale(1.f, 1.f);

        m_Mesh.scale(scale + scale_diff);

		m_ParentLastScale = m_Parent->getScale();*/
    }

    void MeshObject::setParentLastPosition(const sf::Vector2f& position)
    {
        m_ParentLastPosition = position;
    }

    void MeshObject::setParentLastScale(const sf::Vector2f& factor)
    {
        m_ParentLastScale = factor;
    }

    void MeshObject::setParentLastRotation(const float& angle)
    {
        m_ParentLastRotation = angle;
    }

    void MeshObject::setMeshType(Mesh::Type type)
    {
		//m_Mesh.setType(type);
		//m_Mesh.updateColor();
    }

    void MeshObject::setMeshFixedRotation(bool flag)
    {
		//m_Mesh.setFixedRotation(flag);
    }

    void MeshObject::setMeshSensor(bool flag)
    {
		//m_Mesh.setIsSensor(flag);
    }

    void MeshObject::setMeshAllowSleep(bool flag)
    {
		//m_Mesh.setAllowSleep(flag);
    }

    void MeshObject::setMeshDensity(const float& density)
    {
		//m_Mesh.setDensity(density);
    }

    void MeshObject::setMeshFriction(const float& friction)
    {
		//m_Mesh.setFriction(friction);
    }

    void MeshObject::setMeshRestitution(const float& restitution)
    {
		//m_Mesh.setRestitution(restitution);
    }

    void MeshObject::setMeshGravityScale(const float& gravityScale)
    {
		//m_Mesh.setGravityScale(gravityScale);
    }

    bool MeshObject::getMeshFixedRotation() const
    {
		//return m_Mesh.getFixedRotation();
    }

    bool MeshObject::getMeshSensor() const
    {
		//return m_Mesh.getIsSensor();
    }

    bool MeshObject::getMeshAllowSleep() const
    {
		//return m_Mesh.getAllowSleep();
    }

    float MeshObject::getMeshDensity() const
    {
		//return m_Mesh.getDensity();
    }

    float MeshObject::getMeshFriction() const
    {
		//return m_Mesh.getFriction();
    }

    float MeshObject::getMeshRestitution() const
    {
		//return m_Mesh.getRestitution();
    }

    float MeshObject::getMeshGravityScale() const
    {
		//return m_Mesh.getGravityScale();
    }

    nlohmann::json MeshObject::toJson() const
    {
        nlohmann::json mesh_json;

        mesh_json = Object::toJson();
		//mesh_json["mesh"] = m_Mesh.toJson();

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

        mesh_object->setParentLastPosition(getPosition() - position);
        mesh_object->setId(-1);
		//mesh_object->getMesh()->setMeshId(-1);

        return mesh_object;
    }

    MeshObject::Ptr MeshObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::Mesh_Object)
            return nullptr;

        return  std::static_pointer_cast<MeshObject>(object);
    }

}
