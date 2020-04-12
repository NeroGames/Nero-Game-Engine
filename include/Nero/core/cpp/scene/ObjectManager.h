////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SCENEOBJECTMANAGER_H
#define SCENEOBJECTMANAGER_H
///////////////////////////HEADERS//////////////////////////
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/object/UIObject.h>
#include <Nero/core/cpp/object/ButtonObject.h>
#include <Nero/core/cpp/object/PhysicObject.h>
#include <Nero/core/cpp/object/LayerObject.h>
#include <Nero/core/cpp/model/FrontScreen.h>
#include <Nero/core/cpp/model/JointProperty.h>
#include <Nero/core/cpp/model/DistanceJoint.h>
#include <Nero/core/cpp/model/FrictionJoint.h>
#include <Nero/core/cpp/model/GearJoint.h>
#include <Nero/core/cpp/model/MotorJoint.h>
#include <Nero/core/cpp/model/MouseJoint.h>
#include <Nero/core/cpp/model/PrismaticJoint.h>
#include <Nero/core/cpp/model/PulleyJoint.h>
#include <Nero/core/cpp/model/RevoluteJoint.h>
#include <Nero/core/cpp/model/RopeJoint.h>
#include <Nero/core/cpp/model/WeldJoint.h>
#include <Nero/core/cpp/model/WheelJoint.h>
//BOX2D
#include <Box2D/Dynamics/b2World.h>
//STD
#include <assert.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class ObjectManager
    {
        public:
            typedef std::vector<Object::Ptr>        ObjectTab;
            typedef std::vector<PhysicObject::Ptr>  PhysicObjectTab;
            typedef std::shared_ptr<ObjectManager>  Ptr;

        public:
                                            ObjectManager(Object::Ptr root_object, b2World* world, std::vector<Screen::Ptr>& screenTable);
                                           ~ObjectManager();

            //Find by name
            Object::Ptr                     findObject(sf::String name);
            Object::Ptr                     findLayerObject(sf::String name);
            Object::Ptr                     findObjectInLayer(sf::String name, sf::String layer);
            Object::Ptr                     findSpriteObject(sf::String name);
            Object::Ptr                     findPhysicObject(sf::String name);
            Object::Ptr                     findSolidObject(sf::String name);

            Object::Ptr                     findScreenUIObject(std::string screenName, std::string ObjectName);
            Object::Ptr                     findScreenObject(std::string screenName, std::string ObjectName);

            //Find by id
            Object::Ptr                     findObject(int id);
            Object::Ptr                     findLayerObject(int id);
            Object::Ptr                     findObjectInLayer(int id, sf::String layer);
            Object::Ptr                     findSpriteObject(int id);
            Object::Ptr                     findPhysicObject(int id);
            Object::Ptr                     findSolidObject(int id);
            //Move
            Object::Ptr                     moveObject(sf::String name);
            Object::Ptr                     movePhysicObject(sf::String name);
            Object::Ptr                     moveSpriteObject(sf::String name);
            Object::Ptr                     moveSolidObject(sf::String name);
            //Remove
            bool                            removeObject(Object::Ptr object);
            bool                            removeObject(sf::String name);
            bool                            removeLayer(std::string name);
            //
            void                            checkWorldObject(std::vector<sf::String> objectNameTab);
            void                            checkScreenObject(std::string screen, std::vector<sf::String> objectNameTab);
            //
            void                            removeDeadPhysicObject();

            void                            setPhysicWorld(b2World* world);
            //
            void                            addSpriteObject(Object::Ptr object);
            void                            addPhysicObject(Object::Ptr object);
            void                            addSolidObject(Object::Ptr object);
            void                            addObject(Object::Ptr object);

            //joints
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, DistanceJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, FrictionJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, MotorJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, MouseJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, RopeJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, WeldJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, WheelJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, PulleyJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, PrismaticJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, RevoluteJointProperty property);
            void                            createJoint(Object::Ptr objectA, Object::Ptr objectB, GearJointProperty property);
            //
            PhysicJoint::Ptr                findJoint(int jointId);
            PhysicJoint::Ptr                findJoint(const std::string& jointName);
            bool                            removeJoint(int jointId);
            bool                            removeJoint(const std::string& jointName);

            int                             getNewObjectId();
            void                            setObjectCount(int count);

        private:

            Object::Ptr                     findObject(Object::Ptr object, sf::String name);
            void                            findObject(Object::Ptr object, sf::String name, Object::Ptr& result);
            Object::Ptr                     findChildObject(Object::Ptr object, sf::String name);
            Object::Ptr                     findObjectByLayerType(sf::String name, Object::Type layer_type);

            //
            Object::Ptr                     findObject(Object::Ptr object, int id);
            void                            findObject(Object::Ptr object, int id, Object::Ptr& result);
            Object::Ptr                     findChildObject(Object::Ptr object, int id);
            Object::Ptr                     findObjectByLayerType(int id, Object::Type layer_type);

            //
            Object::Ptr                     moveObjectByLayerType(sf::String name, Object::Type layer_type);
            Object::Ptr                     moveChildObject(Object::Ptr object, sf::String name);

            //
            bool                            removeObjectByLayerType(Object::Ptr object, Object::Type layer_type);
            bool                            removeChildObject(Object::Ptr object, Object::Ptr child);

        private:
            Object::Ptr                     m_RootObject;
            b2World*                        m_PhysicWorld;
            PhysicObjectTab                 m_DeadPhysicObject;
            std::vector<Screen::Ptr>&       m_ScreenTable;
            LayerObject::Ptr                m_CustomLayer;
            int                             m_JointCount;
            int                             m_ObjectCount;
            std::vector<PhysicJoint::Ptr>   m_PhysicJointTable;
    };
}
#endif // SCENEOBJECTMANAGER_H
