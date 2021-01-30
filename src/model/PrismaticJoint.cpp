////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/PrismaticJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    PrismaticJoint::PrismaticJoint():
         PhysicJoint()
        ,m_Joint(nullptr)
    {
        m_Type = PhysicJoint::Prismatic_Joint;
    }

    PrismaticJoint::~PrismaticJoint()
    {
        //dtor
    }

    void PrismaticJoint::setJoint(b2PrismaticJoint* joint)
    {
        m_Joint = joint;
    }

    b2PrismaticJoint* PrismaticJoint::getJoint() const
    {
        return m_Joint;
    }

    PrismaticJoint::Ptr PrismaticJoint::Cast(PhysicJoint::Ptr joint)
    {
        if(joint->getType() != PhysicJoint::Prismatic_Joint)
            return nullptr;

        return  std::static_pointer_cast<PrismaticJoint>(joint);
    }

    b2Joint* PrismaticJoint::getGenericJoint()
    {
        return (b2Joint*)m_Joint;
    }
}

