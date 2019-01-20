#include <Nero/scene/ObjectManager.h>

#include <Nero/utility/Utility.h>

namespace nero
{
    ObjectManager::ObjectManager(Object::Ptr root_object, b2World* world,  std::vector<Screen::Ptr>& screenTable):
        m_RootObject(root_object)
        ,m_World(world)
        ,m_DeadPhysicObject()
        ,m_ScreenTable(screenTable)
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
        m_World = nullptr;
        delete m_World;
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
            if((*object)->isSensor())
            {
                m_World->DestroyBody((*object)->getBody());
                removeObject((*object));
            }
        }

        m_DeadPhysicObject.clear();
    }

    void ObjectManager::setWorld(b2World* world)
    {
        m_World = world;
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

}

