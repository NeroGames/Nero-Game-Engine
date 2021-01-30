////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/ObjectManager.h>
#include <Nero/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ObjectManager::ObjectManager(Object::Ptr root_object, b2World* world,  std::vector<Screen::Ptr>& screenTable):
         m_RootObject(root_object)
        ,m_PhysicWorld(world)
        ,m_DeadPhysicObject()
        ,m_ScreenTable(screenTable)
        ,m_JointCount(0)
        ,m_ObjectCount(0)
    {
        m_CustomLayer =  LayerObject::Ptr(new LayerObject());
        m_CustomLayer->setId(-1);
        m_CustomLayer->setIsVisible(true);
        m_CustomLayer->setIsSelected(false);
        m_CustomLayer->setName("custom");
        m_CustomLayer->setOrder(0);
        m_RootObject->addChild(m_CustomLayer);

    }

    ObjectManager::~ObjectManager()
    {
        m_PhysicWorld = nullptr;
        delete m_PhysicWorld;
    }


    void ObjectManager::findObject(Object::Ptr object, sf::String name, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getName() == name)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for (auto it = childTab->begin(); it != childTab->end(); it++)
                findObject(*it, name, result);
        }
    }

    Object::Ptr ObjectManager::findObject(Object::Ptr object, sf::String name)
    {
        Object::Ptr result = nullptr;
        findObject(object, name, result);

        return result;
    }

    Object::Ptr ObjectManager::findObject(sf::String name)
    {
        return findObject(m_RootObject, name);
    }

    Object::Ptr ObjectManager::findChildObject(Object::Ptr object, sf::String name)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getName() == name)
                return (*it);

        return nullptr;
    }

    Object::Ptr ObjectManager::findLayerObject(sf::String name)
    {
        return findChildObject(m_RootObject, name);
    }

    Object::Ptr ObjectManager::findObjectByLayerType(sf::String name, Object::Type layer_type)
    {
        Object::Ptr result = nullptr;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
            if((*it)->getSecondType() != layer_type)
                continue;

             result = findChildObject(*it, name);

             if(result)
                break;
        }

        return result;
    }

    Object::Ptr ObjectManager::findSpriteObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Sprite_Object);
    }

    Object::Ptr ObjectManager::findPhysicObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Physic_Object);
    }

    Object::Ptr ObjectManager::findSolidObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Solid_Object);
    }

    Object::Ptr ObjectManager::findTextObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Text_Object);
    }

    Object::Ptr ObjectManager::findAnimationObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Animation_Object);
    }

    Object::Ptr ObjectManager::findSolidAnimationObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Animation_Solid_Object);
    }

    Object::Ptr ObjectManager::findObjectInLayer(sf::String name, sf::String layer)
    {
        return findChildObject(findLayerObject(layer), name);
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    //Find by Object_Id
     void ObjectManager::findObject(Object::Ptr object, int id, Object::Ptr& result)
    {
        if(result != nullptr)
            return;

        if(object->getId() == id)
            result = object;
        else
        {
            auto childTab = object->getAllChild();
            for (auto it = childTab->begin(); it != childTab->end(); it++)
                findObject(*it, id, result);
        }
    }

    Object::Ptr ObjectManager::findObject(Object::Ptr object, int id)
    {
        Object::Ptr result = nullptr;
        findObject(object, id, result);

        return result;
    }

    Object::Ptr ObjectManager::findObject(int id)
    {
        return findObject(m_RootObject, id);
    }

    Object::Ptr ObjectManager::findChildObject(Object::Ptr object, int id)
    {
        if(!object) return nullptr;

        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getId() == id)
                return (*it);

        return nullptr;
    }

    Object::Ptr ObjectManager::findLayerObject(int id)
    {
        return findChildObject(m_RootObject, id);
    }

    Object::Ptr ObjectManager::findObjectByLayerType(int id, Object::Type layer_type)
    {
        Object::Ptr result = nullptr;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
            if((*it)->getSecondType() != layer_type)
                continue;

             result = findChildObject(*it, id);

             if(result)
                break;
        }

        return result;
    }

    Object::Ptr ObjectManager::findSpriteObject(int id)
    {
        return findObjectByLayerType(id, Object::Sprite_Object);
    }

    Object::Ptr ObjectManager::findPhysicObject(int id)
    {
        return findObjectByLayerType(id, Object::Physic_Object);
    }

    Object::Ptr ObjectManager::findSolidObject(int id)
    {
        return findObjectByLayerType(id, Object::Solid_Object);
    }

    Object::Ptr ObjectManager::findTextObject(int id)
    {
        return findObjectByLayerType(id, Object::Text_Object);
    }

    Object::Ptr ObjectManager::findAnimationObject(int id)
    {
        return findObjectByLayerType(id, Object::Animation_Object);
    }

    Object::Ptr ObjectManager::findSolidAnimationObject(int id)
    {
        return findObjectByLayerType(id, Object::Animation_Solid_Object);
    }

    Object::Ptr ObjectManager::findObjectInLayer(int id, sf::String layer)
    {
        return findChildObject(findLayerObject(layer), id);
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    //Checking
    void ObjectManager::checkWorldObject(std::vector<sf::String> objectNameTab)
    {
        for(sf::String object_name : objectNameTab)
        {
            if(!findObject(object_name))
            {
                throw std::runtime_error("Object [" + _s(object_name) + "] not found, please check that you have entered the correct object name");
            }
        }
    }

    void ObjectManager::checkScreenObject(std::string screen, std::vector<sf::String> objectNameTab)
    {
        for(sf::String object_name : objectNameTab)
        {
            if(!findScreenUIObject(screen, object_name) && !findScreenObject(screen, object_name))
            {
                throw std::runtime_error("Object [" + _s(object_name) + "] not found in Screen [" + _s(screen) + "], please check that you have entered the correct object name");
            }
        }
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    //Move object by name
    Object::Ptr ObjectManager::moveObject(sf::String name)
    {
        Object::Ptr result = nullptr;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
             result = moveChildObject(*it, name);

             if(result)
                break;
        }

        return result;
    }

    Object::Ptr ObjectManager::moveObjectByLayerType(sf::String name, Object::Type layer_type)
    {
        Object::Ptr result = nullptr;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
            if((*it)->getSecondType() != layer_type)
                continue;

             result = moveChildObject(*it, name);

             if(result)
                break;
        }

        return result;
    }


    Object::Ptr ObjectManager::moveChildObject(Object::Ptr object, sf::String name)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getName() == name)
            {
                auto result = (*it);

                object->removeChild((*it));

                return result;
            }

        return nullptr;
    }

    Object::Ptr ObjectManager::movePhysicObject(sf::String name)
    {
        return moveObjectByLayerType(name, Object::Physic_Object);
    }

    Object::Ptr ObjectManager::moveSpriteObject(sf::String name)
    {
        return moveObjectByLayerType(name, Object::Sprite_Object);
    }

    Object::Ptr ObjectManager::moveSolidObject(sf::String name)
    {
        return moveObjectByLayerType(name, Object::Solid_Object);
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    bool ObjectManager::removeChildObject(Object::Ptr object,  Object::Ptr child)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
        {
            if((*it)->getId() == child->getId())
            {
                if((object->getSecondType() == Object::Physic_Object || object->getSecondType() == Object::Solid_Object || object->getSecondType() == Object::Animation_Solid_Object) &&
                   (*it)->getSecondType() != Object::None)
                {
                    PhysicObject::Ptr physic_Object = PhysicObject::Cast(*it);
                    physic_Object->setSensor(true);
                    physic_Object->setDead(true);
                    physic_Object->setSecondType(Object::None);
                    m_DeadPhysicObject.push_back(physic_Object);

                }
                else
                {
                    object->removeChild((*it));
                }

                return true;
            }
        }


        return false;
    }

    bool ObjectManager::removeObject(Object::Ptr object)
    {
        if(!object)
            return false;

        bool result = true;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
             result = removeChildObject(*it, object);

             if(result)
                break;
        }

        return result;
    }

    bool ObjectManager::removeObject(sf::String name)
    {
        return removeObject(findObject(name));
    }

    void ObjectManager::removeDeadPhysicObject()
    {
        if(m_DeadPhysicObject.empty())
            return;

        for(auto object = m_DeadPhysicObject.begin(); object != m_DeadPhysicObject.end(); object++)
        {
            if((*object)->isDead())
            {
                m_PhysicWorld->DestroyBody((*object)->getBody());
                removeObject((*object));
            }
        }

        m_DeadPhysicObject.clear();
    }

    void ObjectManager::setPhysicWorld(b2World* world)
    {
        m_PhysicWorld = world;
    }

    void ObjectManager::addObject(Object::Ptr object)
    {
        m_CustomLayer->addChild(object);
    }

    Object::Ptr ObjectManager::findScreenUIObject(std::string screenName, std::string ObjectName)
    {
        auto screen = std::find_if(m_ScreenTable.begin(), m_ScreenTable.end(), [&](Screen::Ptr screen){return screen->name == screenName;});

        return findObject((*screen)->screenUI, ObjectName);
    }

    Object::Ptr ObjectManager::findScreenObject(std::string screenName, std::string ObjectName)
    {
        auto screen = std::find_if(m_ScreenTable.begin(), m_ScreenTable.end(), [&](Screen::Ptr screen){return screen->name == screenName;});

        return findObject((*screen)->screen, ObjectName);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, DistanceJointProperty property)
    {
        DistanceJoint::Ptr joint = DistanceJoint::Ptr(new DistanceJoint());
        joint->setJointId(++m_JointCount);

        b2DistanceJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.length = property.length / SCALE;
        jointDef.frequencyHz = property.frequencyHz;
        jointDef.dampingRatio = property.dampingRatio;

        joint->setJoint((b2DistanceJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, FrictionJointProperty property)
    {
        FrictionJoint::Ptr joint = FrictionJoint::Ptr(new FrictionJoint());
        joint->setJointId(++m_JointCount);

        b2FrictionJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.maxForce = property.maxForce;
        jointDef.maxTorque = property.maxTorque;

        joint->setJoint((b2FrictionJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, MotorJointProperty property)
    {
        MotorJoint::Ptr joint = MotorJoint::Ptr(new MotorJoint());
        joint->setJointId(++m_JointCount);

        b2MotorJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.linearOffset = sf_to_b2(property.linearOffset, SCALE);
        jointDef.angularOffset = toRadian(property.angularOffset);
        jointDef.maxForce = property.maxForce;
        jointDef.maxTorque = property.maxTorque;
        jointDef.correctionFactor = property.correctionFactor;

        joint->setJoint((b2MotorJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, MouseJointProperty property)
    {
        MouseJoint::Ptr joint = MouseJoint::Ptr(new MouseJoint());
        joint->setJointId(++m_JointCount);

        b2MouseJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.target = sf_to_b2(property.target, SCALE);;
        jointDef.maxForce = property.maxForce;
        jointDef.frequencyHz = property.frequencyHz;
        jointDef.dampingRatio = property.dampingRatio;

        joint->setJoint((b2MouseJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, RopeJointProperty property)
    {
        RopeJoint::Ptr joint = RopeJoint::Ptr(new RopeJoint());
        joint->setJointId(++m_JointCount);

        b2RopeJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.maxLength = property.maxLength / SCALE;

        joint->setJoint((b2RopeJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, WeldJointProperty property)
    {
        WeldJoint::Ptr joint = WeldJoint::Ptr(new WeldJoint());
        joint->setJointId(++m_JointCount);

        b2WeldJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.referenceAngle = toRadian(property.referenceAngle);
        jointDef.frequencyHz = property.frequencyHz;
        jointDef.dampingRatio = property.dampingRatio;

        joint->setJoint((b2WeldJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, WheelJointProperty property)
    {
        WheelJoint::Ptr joint = WheelJoint::Ptr(new WheelJoint());
        joint->setJointId(++m_JointCount);

        b2WheelJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.localAxisA = sf_to_b2(property.localAxisA, SCALE);
        jointDef.enableMotor = property.enableMotor;
        jointDef.maxMotorTorque = property.maxMotorTorque;
        jointDef.motorSpeed = property.motorSpeed;
        jointDef.frequencyHz = property.frequencyHz;
        jointDef.dampingRatio = property.dampingRatio;

        joint->setJoint((b2WheelJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, PulleyJointProperty property)
    {
        PulleyJoint::Ptr joint = PulleyJoint::Ptr(new PulleyJoint());
        joint->setJointId(++m_JointCount);

        b2PulleyJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.groundAnchorA = sf_to_b2(property.groundAnchorA, SCALE);
        jointDef.groundAnchorB = sf_to_b2(property.groundAnchorB, SCALE);
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.lengthA = property.lengthA/SCALE;
        jointDef.lengthB = property.lengthB/SCALE;
        jointDef.ratio = property.ratio;

        joint->setJoint((b2PulleyJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, PrismaticJointProperty property)
    {
        PrismaticJoint::Ptr joint = PrismaticJoint::Ptr(new PrismaticJoint());
        joint->setJointId(++m_JointCount);

        b2PrismaticJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.localAxisA = sf_to_b2(property.localAxisA, SCALE);
        jointDef.referenceAngle = toRadian(property.referenceAngle);
        jointDef.enableLimit = property.enableLimit;
        jointDef.lowerTranslation = property.lowerTranslation/SCALE;
        jointDef.upperTranslation = property.upperTranslation/SCALE;
        jointDef.enableMotor = property.enableMotor;
        jointDef.maxMotorForce = property.maxMotorTorque;
        jointDef.motorSpeed = property.motorSpeed;

        joint->setJoint((b2PrismaticJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, RevoluteJointProperty property)
    {
        RevoluteJoint::Ptr joint = RevoluteJoint::Ptr(new RevoluteJoint());
        joint->setJointId(++m_JointCount);

        b2RevoluteJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.localAnchorA = sf_to_b2(property.localAnchorA, SCALE);
        jointDef.localAnchorB = sf_to_b2(property.localAnchorB, SCALE);
        jointDef.referenceAngle = toRadian(property.referenceAngle);
        jointDef.enableLimit = property.enableLimit;
        jointDef.lowerAngle = toRadian(property.lowerAngle);
        jointDef.upperAngle = toRadian(property.upperAngle);
        jointDef.enableMotor = property.enableMotor;
        jointDef.motorSpeed = property.motorSpeed;
        jointDef.maxMotorTorque = property.maxMotorTorque;

        joint->setJoint((b2RevoluteJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    void ObjectManager::createJoint(Object::Ptr objectA, Object::Ptr objectB, GearJointProperty property)
    {
        PhysicJoint::Ptr jointA = findJoint(property.jointAId);
        PhysicJoint::Ptr jointB = findJoint(property.jointBId);

        if(jointA->getType() != PhysicJoint::Prismatic_Joint && jointA->getType() != PhysicJoint::Revolute_Joint) return;
        if(jointB->getType() != PhysicJoint::Prismatic_Joint && jointB->getType() != PhysicJoint::Revolute_Joint) return;

        GearJoint::Ptr joint = GearJoint::Ptr(new GearJoint());
        joint->setJointId(++m_JointCount);

        b2GearJointDef jointDef;
        jointDef.bodyA = PhysicObject::Cast(objectA)->getBody();
        jointDef.bodyB = PhysicObject::Cast(objectB)->getBody();
        jointDef.collideConnected = property.collideConnected;
        jointDef.userData = (void *)joint->getJointId();
        jointDef.joint1 = jointA->getType() == PhysicJoint::Prismatic_Joint ? (b2Joint*)PrismaticJoint::Cast(jointA)->getJoint() : (b2Joint*)RevoluteJoint::Cast(jointA)->getJoint();
        jointDef.joint2 = jointB->getType() == PhysicJoint::Prismatic_Joint ? (b2Joint*)PrismaticJoint::Cast(jointB)->getJoint() : (b2Joint*)RevoluteJoint::Cast(jointB)->getJoint();
        jointDef.ratio  = property.ratio;

        joint->setJoint((b2GearJoint*)m_PhysicWorld->CreateJoint(&jointDef));

        joint->setName(property.name);
        joint->setObjectAId(objectA->getId());
        joint->setObjectBId(objectB->getId());
        joint->setObjectAName(objectA->getName());
        joint->setObjectBName(objectB->getName());

        m_PhysicJointTable.push_back(joint);
    }

    PhysicJoint::Ptr ObjectManager::findJoint(int jointId)
    {
        for(PhysicJoint::Ptr joint : m_PhysicJointTable)
        {
            if(joint->getJointId() == jointId)
                return joint;
        }

        return nullptr;
    }

    PhysicJoint::Ptr ObjectManager::findJoint(const std::string& jointName)
    {
        for(PhysicJoint::Ptr joint : m_PhysicJointTable)
        {
            if(joint->getName() == jointName)
                return joint;
        }

        return nullptr;
    }

    bool ObjectManager::removeJoint(int jointId)
    {
        for(PhysicJoint::Ptr joint : m_PhysicJointTable)
        {
            if(joint->getJointId() == jointId)
            {
                return true;
            }
        }

        return false;
    }

    bool ObjectManager::removeJoint(const std::string& jointName)
    {
        int i = 0;
        for(PhysicJoint::Ptr joint : m_PhysicJointTable)
        {
            if(joint->getName() == jointName)
            {
                m_PhysicWorld->DestroyJoint(joint->getGenericJoint());
                m_PhysicJointTable.erase(m_PhysicJointTable.begin() + i);

                return true;
            }

            i++;
        }

        return false;
    }

    bool ObjectManager::removeLayer(std::string name)
    {
        auto layer = findLayerObject(name);

        if(!layer) return false;

        auto childTab = layer->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            removeObject(*it);

        return true;
    }

    int ObjectManager::getNewObjectId()
    {
        return ++m_ObjectCount;
    }

    int ObjectManager::getNewJointId()
    {
        return ++m_JointCount;
    }

    void ObjectManager::setObjectCount(int count)
    {
        m_ObjectCount = count;
    }

    void ObjectManager::disableLayer(const std::string& name)
    {
        Object::Ptr layer = findLayerObject(name);

        if(!layer)
            return;

        layer->setIsVisible(false);
        layer->setIsUpdateable(false);

        if(layer->getSecondType() == Object::Physic_Object || layer->getSecondType() == Object::Solid_Object || layer->getSecondType() == Object::Animation_Solid_Object)
        {
            auto childTable = layer->getAllChild();
            for(Object::Ptr object : *childTable)
            {
                PhysicObject::Cast(object)->setActive(false);
            }
        }
    }

    void ObjectManager::enableLayer(const std::string& name)
    {
        Object::Ptr layer = findLayerObject(name);

        if(!layer)
            return;

        layer->setIsVisible(true);
        layer->setIsUpdateable(true);

        if(layer->getSecondType() == Object::Physic_Object || layer->getSecondType() == Object::Solid_Object || layer->getSecondType() == Object::Animation_Solid_Object)
        {
            auto childTable = layer->getAllChild();
            for(Object::Ptr object : *childTable)
            {
                PhysicObject::Cast(object)->setActive(true);
            }
        }
    }
}

