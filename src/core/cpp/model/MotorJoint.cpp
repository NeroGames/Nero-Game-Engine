////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/model/MotorJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    MotorJoint::MotorJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Motor_Joint;
    }

    MotorJoint::~MotorJoint()
    {
        //dtor
    }

    void MotorJoint::setJoint(b2MotorJoint* joint)
    {
        m_Joint = joint;
    }

    b2MotorJoint* MotorJoint::getJoint() const
    {
        return m_Joint;
    }

    MotorJoint::Ptr MotorJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Motor_Joint)
            return nullptr;

        return  std::static_pointer_cast<MotorJoint>(joint);
    }

    b2Joint* MotorJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}
