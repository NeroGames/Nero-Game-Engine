////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/GearJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    GearJoint::GearJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Gear_Joint;
    }

    GearJoint::~GearJoint()
    {
        //dtor
    }

    void GearJoint::setJoint(b2GearJoint* joint)
    {
        m_Joint = joint;
    }

    b2GearJoint* GearJoint::getJoint() const
    {
        return m_Joint;
    }

    GearJoint::Ptr GearJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Gear_Joint)
            return nullptr;

        return  std::static_pointer_cast<GearJoint>(joint);
    }

    b2Joint* GearJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}


