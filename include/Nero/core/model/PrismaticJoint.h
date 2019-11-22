////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PRISMATICJOINT_H
#define PRISMATICJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2PrismaticJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class PrismaticJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<PrismaticJoint>     Ptr;
            static  Ptr                                 Cast(PhysicJoint::Ptr joint);

                                    PrismaticJoint();
            virtual                ~PrismaticJoint();

            void                    setJoint(b2PrismaticJoint* joint);
            b2PrismaticJoint*       getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2PrismaticJoint*        m_Joint;
    };
}
#endif // PRISMATICJOINT_H
