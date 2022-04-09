////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
#ifndef MOUSEJOINT_H
#define MOUSEJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class MouseJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<MouseJoint>     Ptr;
            static  Ptr                             Cast(PhysicJoint::Ptr joint);

                                    MouseJoint();
            virtual                ~MouseJoint();

            void                    setJoint(b2MouseJoint* joint);
            b2MouseJoint*           getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2MouseJoint*           m_Joint;
    };
}
#endif // MOUSEJOINT_H
