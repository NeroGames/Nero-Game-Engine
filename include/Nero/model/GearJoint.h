////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GEARJOINT_H
#define GEARJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2GearJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class GearJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<GearJoint>      Ptr;
            static  Ptr                             Cast(PhysicJoint::Ptr joint);

                                    GearJoint();
            virtual                ~GearJoint();

            void                    setJoint(b2GearJoint* joint);
            b2GearJoint*            getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2GearJoint*            m_Joint;
    };
}
#endif // GEARJOINT_H
