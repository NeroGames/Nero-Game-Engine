////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef MOTORJOINT_H
#define MOTORJOINT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/model/PhysicJoint.h>
// BOX2D
#include <Box2D/Dynamics/Joints/b2MotorJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class MotorJoint : public PhysicJoint
    {
      public:
        typedef std::shared_ptr<MotorJoint> Ptr;
        static Ptr                          Cast(PhysicJoint::Ptr joint);

        MotorJoint();
        virtual ~MotorJoint();

        void             setJoint(b2MotorJoint* joint);
        b2MotorJoint*    getJoint() const;
        virtual b2Joint* getGenericJoint();

      private:
        b2MotorJoint* m_Joint;
    };
} // namespace nero
#endif // MOTORJOINT_H
