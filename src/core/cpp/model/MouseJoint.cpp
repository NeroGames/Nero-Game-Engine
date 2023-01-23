////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/MouseJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    MouseJoint::MouseJoint()
        : PhysicJoint()
        , m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Mouse_Joint;
    }

    MouseJoint::~MouseJoint()
    {
        // dtor
    }

    void MouseJoint::setJoint(b2MouseJoint* joint)
    {
        m_Joint = joint;
    }

    b2MouseJoint* MouseJoint::getJoint() const
    {
        return m_Joint;
    }

    MouseJoint::Ptr MouseJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Mouse_Joint)
            return nullptr;

        return std::static_pointer_cast<MouseJoint>(joint);
    }

    b2Joint* MouseJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
} // namespace nero
