////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/model/PhysicJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PhysicJoint::PhysicJoint():
         m_JointId(-1)
        ,m_Name("joint")
        ,m_ObjectAId(-1)
        ,m_ObjectBId(-1)
        ,m_ObjectAName("objectA")
        ,m_ObjectBName("objectB")
        ,m_Type(PhysicJoint::None)
    {
        //ctor
    }

    PhysicJoint::~PhysicJoint()
    {
        //dtor
    }

    void PhysicJoint::setJointId(int jointId)
    {
        m_JointId = jointId;
    }

    int PhysicJoint::getJointId() const
    {
        return m_JointId;
    }

    void PhysicJoint::setName(const std::string& name)
    {
        m_Name = name;
    }

    std::string PhysicJoint::getName() const
    {
        return m_Name;
    }

    void PhysicJoint::setObjectAId(int objectId)
    {
        m_ObjectAId = objectId;
    }

    void PhysicJoint::setObjectBId(int objectId)
    {
        m_ObjectBId = objectId;
    }

    void PhysicJoint::setObjectAName(const std::string& name)
    {
        m_ObjectAName = name;
    }

    void PhysicJoint::setObjectBName(const std::string& name)
    {
        m_ObjectBName = name;
    }

    int PhysicJoint::getObjectAId() const
    {
        return m_ObjectAId;
    }

    int PhysicJoint::getObjectBId() const
    {
        return m_ObjectBId;
    }

    std::string PhysicJoint::getObjectAName() const
    {
        return m_ObjectAName;
    }

    std::string PhysicJoint::getObjectBName() const
    {
        return m_ObjectBName;
    }

    PhysicJoint::Type PhysicJoint::getType() const
    {
        return m_Type;
    }

    PhysicJoint::Ptr PhysicJoint::Cast(PhysicJoint::Ptr joint)
    {
        return joint;
    }

    b2Joint* PhysicJoint::getGenericJoint()
    {
        return nullptr;
    }
}


