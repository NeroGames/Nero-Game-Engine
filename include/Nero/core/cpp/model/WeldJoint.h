////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef WELDJOINT_H
#define WELDJOINT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PhysicJoint.h>
// BOX2D
#include <Box2D/Dynamics/Joints/b2WeldJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class WeldJoint : public PhysicJoint
    {
      public:
        typedef std::shared_ptr<WeldJoint> Ptr;
        static Ptr                         Cast(PhysicJoint::Ptr joint);

        WeldJoint();
        virtual ~WeldJoint();

        void             setJoint(b2WeldJoint* joint);
        b2WeldJoint*     getJoint() const;
        virtual b2Joint* getGenericJoint();

      private:
        b2WeldJoint* m_Joint;
    };
} // namespace nero
#endif // WELDJOINT_H
