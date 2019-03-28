////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WHEELJOINT_H
#define WHEELJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2WheelJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WheelJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<WheelJoint>     Ptr;
            static  Ptr                             Cast(PhysicJoint::Ptr joint);

                                    WheelJoint();
            virtual                ~WheelJoint();

            void                    setJoint(b2WheelJoint* joint);
            b2WheelJoint*           getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2WheelJoint*            m_Joint;
    };
}
#endif // WHEELJOINT_H
