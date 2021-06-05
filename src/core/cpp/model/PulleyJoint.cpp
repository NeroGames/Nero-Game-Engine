////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/model/PulleyJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PulleyJoint::PulleyJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Pulley_Joint;
    }

    PulleyJoint::~PulleyJoint()
    {
        //dtor
    }

    void PulleyJoint::setJoint(b2PulleyJoint* joint)
    {
        m_Joint = joint;
    }

    b2PulleyJoint* PulleyJoint::getJoint() const
    {
        return m_Joint;
    }

    PulleyJoint::Ptr PulleyJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Pulley_Joint)
            return nullptr;

        return  std::static_pointer_cast<PulleyJoint>(joint);
    }

    b2Joint* PulleyJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}
