////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef FRICTIONJOINT_H
#define FRICTIONJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2FrictionJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class FrictionJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<FrictionJoint>      Ptr;
            static  Ptr                                 Cast(PhysicJoint::Ptr joint);

                                    FrictionJoint();
            virtual                ~FrictionJoint();

            void                    setJoint(b2FrictionJoint* joint);
            b2FrictionJoint*        getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2FrictionJoint*        m_Joint;
    };
}
#endif // FRICTIONJOINT_H
