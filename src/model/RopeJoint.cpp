////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/RopeJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    RopeJoint::RopeJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Rope_Joint;
    }

    RopeJoint::~RopeJoint()
    {
        //dtor
    }

    void RopeJoint::setJoint(b2RopeJoint* joint)
    {
        m_Joint = joint;
    }

    b2RopeJoint* RopeJoint::getJoint() const
    {
        return m_Joint;
    }

    RopeJoint::Ptr RopeJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Rope_Joint)
            return nullptr;

        return  std::static_pointer_cast<RopeJoint>(joint);
    }

    b2Joint* RopeJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}
