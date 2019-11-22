////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/model/WeldJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    WeldJoint::WeldJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Weld_Joint;
    }

    WeldJoint::~WeldJoint()
    {
        //dtor
    }

    void WeldJoint::setJoint(b2WeldJoint* joint)
    {
        m_Joint = joint;
    }

    b2WeldJoint* WeldJoint::getJoint() const
    {
        return m_Joint;
    }

    WeldJoint::Ptr WeldJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Weld_Joint)
            return nullptr;

        return  std::static_pointer_cast<WeldJoint>(joint);
    }

    b2Joint* WeldJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}
