////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DISTANCEJOINT_H
#define DISTANCEJOINT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PhysicJoint.h>
// BOX2D
#include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class DistanceJoint : public PhysicJoint
    {
      public:
        typedef std::shared_ptr<DistanceJoint> Ptr;
        static Ptr                             Cast(PhysicJoint::Ptr joint);

        DistanceJoint();
        virtual ~DistanceJoint();

        void             setJoint(b2DistanceJoint* joint);
        b2DistanceJoint* getJoint() const;
        virtual b2Joint* getGenericJoint();

      private:
        b2DistanceJoint* m_Joint;
    };
} // namespace nero
#endif // DISTANCEJOINT_H
