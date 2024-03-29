////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef REVOLUTEJOINT_H
#define REVOLUTEJOINT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/model/PhysicJoint.h>
// BOX2D
#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
////////////////////////////////////////////////////////////
namespace nero
{
    class RevoluteJoint : public PhysicJoint
    {
      public:
        typedef std::shared_ptr<RevoluteJoint> Ptr;
        static Ptr                             Cast(PhysicJoint::Ptr joint);

        RevoluteJoint();
        virtual ~RevoluteJoint();

        void             setJoint(b2RevoluteJoint* joint);
        b2RevoluteJoint* getJoint() const;
        virtual b2Joint* getGenericJoint();

      private:
        b2RevoluteJoint* m_Joint;
    };
} // namespace nero
#endif // REVOLUTEJOINT_H
