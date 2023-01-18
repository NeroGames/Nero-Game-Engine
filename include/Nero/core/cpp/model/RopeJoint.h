////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ROPEJOINT_H
#define ROPEJOINT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/PhysicJoint.h>
// BOX2D
#include <Box2D/Dynamics/Joints/b2RopeJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class RopeJoint : public PhysicJoint {
      public:
        typedef std::shared_ptr<RopeJoint> Ptr;
        static Ptr                         Cast(PhysicJoint::Ptr joint);

        RopeJoint();
        virtual ~RopeJoint();

        void             setJoint(b2RopeJoint* joint);
        b2RopeJoint*     getJoint() const;
        virtual b2Joint* getGenericJoint();

      private:
        b2RopeJoint* m_Joint;
    };
} // namespace nero
#endif // ROPEJOINT_H
