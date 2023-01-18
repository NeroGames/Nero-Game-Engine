////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef PHYSICJOINT_H
#define PHYSICJOINT_H
///////////////////////////HEADERS//////////////////////////
// BOX2D
#include <Box2D/Dynamics/Joints/b2Joint.h>
// STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicJoint {
      public:
        enum Type
        {
            None,
            Distance_Joint,
            Friction_Joint,
            Gear_Joint,
            Motor_Joint,
            Mouse_Joint,
            Prismatic_Joint,
            Pulley_Joint,
            Revolute_Joint,
            Rope_Joint,
            Weld_Joint,
            Wheel_Joint
        };
        typedef std::shared_ptr<PhysicJoint> Ptr;
        static Ptr                           Cast(PhysicJoint::Ptr joint);

        PhysicJoint();
        virtual ~PhysicJoint();

        void             setJointId(int jointId);
        int              getJointId() const;
        void             setName(const std::string& name);
        std::string      getName() const;
        void             setObjectAId(int objectId);
        void             setObjectBId(int objectId);
        void             setObjectAName(const std::string& name);
        void             setObjectBName(const std::string& name);

        int              getObjectAId() const;
        int              getObjectBId() const;
        std::string      getObjectAName() const;
        std::string      getObjectBName() const;

        Type             getType() const;
        virtual b2Joint* getGenericJoint();

      protected:
        int         m_JointId;
        std::string m_Name;
        int         m_ObjectAId;
        int         m_ObjectBId;
        std::string m_ObjectAName;
        std::string m_ObjectBName;
        Type        m_Type;
    };
} // namespace nero
#endif // PHYSICJOINT_H
