////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/FrictionJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    FrictionJoint::FrictionJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Friction_Joint;
    }

    FrictionJoint::~FrictionJoint()
    {
        //dtor
    }

    void FrictionJoint::setJoint(b2FrictionJoint* joint)
    {
        m_Joint = joint;
    }

    b2FrictionJoint* FrictionJoint::getJoint() const
    {
        return m_Joint;
    }

    FrictionJoint::Ptr FrictionJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Friction_Joint)
            return nullptr;

        return  std::static_pointer_cast<FrictionJoint>(joint);
    }

    b2Joint* FrictionJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}
