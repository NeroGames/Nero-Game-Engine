////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WELDJOINT_H
#define WELDJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2WeldJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WeldJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<WeldJoint>      Ptr;
            static  Ptr                             Cast(PhysicJoint::Ptr joint);

                                    WeldJoint();
            virtual                ~WeldJoint();

            void                    setJoint(b2WeldJoint* joint);
            b2WeldJoint*            getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2WeldJoint*            m_Joint;
    };
}
#endif // WELDJOINT_H
