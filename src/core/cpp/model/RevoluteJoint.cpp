////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/RevoluteJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    RevoluteJoint::RevoluteJoint()
        : PhysicJoint()
        , m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Revolute_Joint;
    }

    RevoluteJoint::~RevoluteJoint()
    {
        // dtor
    }

    void RevoluteJoint::setJoint(b2RevoluteJoint* joint)
    {
        m_Joint = joint;
    }

    b2RevoluteJoint* RevoluteJoint::getJoint() const
    {
        return m_Joint;
    }

    RevoluteJoint::Ptr RevoluteJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Revolute_Joint)
            return nullptr;

        return std::static_pointer_cast<RevoluteJoint>(joint);
    }

    b2Joint* RevoluteJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
} // namespace nero
