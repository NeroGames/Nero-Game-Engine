////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/DistanceJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    DistanceJoint::DistanceJoint()
        : PhysicJoint()
        , m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Distance_Joint;
    }

    ////////////////////////////////////////////////////////////
    DistanceJoint::~DistanceJoint()
    {
        // dtor
    }

    ////////////////////////////////////////////////////////////
    void DistanceJoint::setJoint(b2DistanceJoint* joint)
    {
        m_Joint = joint;
    }

    ////////////////////////////////////////////////////////////
    b2DistanceJoint* DistanceJoint::getJoint() const
    {
        return m_Joint;
    }

    ////////////////////////////////////////////////////////////
    DistanceJoint::Ptr DistanceJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Distance_Joint)
            return nullptr;

        return std::static_pointer_cast<DistanceJoint>(joint);
    }

    b2Joint* DistanceJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
} // namespace nero
