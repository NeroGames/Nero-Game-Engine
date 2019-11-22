////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PULLEYJOINT_H
#define PULLEYJOINT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/model/PhysicJoint.h>
//BOX2D
#include <Box2D/Dynamics/Joints/b2PulleyJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class PulleyJoint : public PhysicJoint
    {
        public:
            typedef std::shared_ptr<PulleyJoint>    Ptr;
            static  Ptr                             Cast(PhysicJoint::Ptr joint);

                                    PulleyJoint();
            virtual                ~PulleyJoint();

            void                    setJoint(b2PulleyJoint* joint);
            b2PulleyJoint*          getJoint() const;
            virtual b2Joint*        getGenericJoint();

        private:
            b2PulleyJoint*          m_Joint;
    };
}
#endif // PULLEYJOINT_H
