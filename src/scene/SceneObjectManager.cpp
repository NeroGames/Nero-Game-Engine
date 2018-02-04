#include <NERO/scene/SceneObjectManager.h>

#include <NERO/Utility.h>

namespace nero
{
    SceneObjectManager::SceneObjectManager(Object::Ptr root_object, b2World* world):
        m_RootObject(root_object),
        m_World(world),
        m_DeadPhysicObject()
    {
        //ctor
    }

    SceneObjectManager::~SceneObjectManager()
    {
        m_World = nullptr;
        delete m_World;
    }


    void SceneObjectManager::findObject(Object::Ptr object, sf::String name, Object::Ptr& result)
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

    Object::Ptr SceneObjectManager::findObject(Object::Ptr object, sf::String name)
    {
        Object::Ptr result = nullptr;
        findObject(object, name, result);

        return result;
    }

    Object::Ptr SceneObjectManager::findObject(sf::String name)
    {
        return findObject(m_RootObject, name);
    }

    Object::Ptr SceneObjectManager::findChildObject(Object::Ptr object, sf::String name)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getName() == name)
                return (*it);

        return nullptr;
    }

    Object::Ptr SceneObjectManager::findLayerObject(sf::String name)
    {
        return findChildObject(m_RootObject, name);
    }

    Object::Ptr SceneObjectManager::findObjectByLayerType(sf::String name, Object::Type layer_type)
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

    Object::Ptr SceneObjectManager::findSpriteObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Sprite_Object);
    }

    Object::Ptr SceneObjectManager::findPhysicObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Physic_Object);
    }

    Object::Ptr SceneObjectManager::findSolidObject(sf::String name)
    {
        return findObjectByLayerType(name, Object::Solid_Object);
    }

    Object::Ptr SceneObjectManager::findObjectInLayer(sf::String name, sf::String layer)
    {
        return findChildObject(findLayerObject(layer), name);
    }

    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    //Find by Object_Id
     void SceneObjectManager::findObject(Object::Ptr object, int id, Object::Ptr& result)
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

    Object::Ptr SceneObjectManager::findObject(Object::Ptr object, int id)
    {
        Object::Ptr result = nullptr;
        findObject(object, id, result);

        return result;
    }

    Object::Ptr SceneObjectManager::findObject(int id)
    {
        return findObject(m_RootObject, id);
    }

    Object::Ptr SceneObjectManager::findChildObject(Object::Ptr object, int id)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getId() == id)
                return (*it);

        return nullptr;
    }

    Object::Ptr SceneObjectManager::findLayerObject(int id)
    {
        return findChildObject(m_RootObject, id);
    }

    Object::Ptr SceneObjectManager::findObjectByLayerType(int id, Object::Type layer_type)
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

    Object::Ptr SceneObjectManager::findSpriteObject(int id)
    {
        return findObjectByLayerType(id, Object::Sprite_Object);
    }

    Object::Ptr SceneObjectManager::findPhysicObject(int id)
    {
        return findObjectByLayerType(id, Object::Physic_Object);
    }

    Object::Ptr SceneObjectManager::findSolidObject(int id)
    {
        return findObjectByLayerType(id, Object::Solid_Object);
    }

    Object::Ptr SceneObjectManager::findObjectInLayer(int id, sf::String layer)
    {
        return findChildObject(findLayerObject(layer), id);
    }

    ///////////////////////////////////////////////////////////////

    void SceneObjectManager::checkAllObject(ObjectTab object_tab)
    {
        for(auto object : object_tab)
            assert(object);
    }

    Object::Ptr SceneObjectManager::moveObjectByLayerType(sf::String name, Object::Type layer_type)
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

    Object::Ptr SceneObjectManager::moveChildObject(Object::Ptr object, sf::String name)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
            if((*it)->getName() == name)
            {
                auto clone = (*it)->clone();

                object->removeChild((*it));

                return clone;
            }

        return nullptr;
    }

    Object::Ptr SceneObjectManager::moveSpriteObject(sf::String name)
    {
        return moveObjectByLayerType(name, Object::Sprite_Object);
    }

    bool SceneObjectManager::removeObjectByLayerType(Object::Ptr object, Object::Type layer_type)
    {
        bool result = true;

        auto layerTab = m_RootObject->getAllChild();

        for (auto it = layerTab->begin(); it != layerTab->end(); it++)
        {
            if((*it)->getSecondType() != layer_type)
                continue;

             result = removeChildObject(*it, object);

             if(result)
                break;
        }

        return result;
    }

    bool SceneObjectManager::removeChildObject(Object::Ptr object,  Object::Ptr child)
    {
        auto childTab = object->getAllChild();

        for (auto it = childTab->begin(); it != childTab->end(); it++)
        {
            if((*it)->getId() == child->getId())
            {
                if(object->getSecondType() == Object::Physic_Object || object->getSecondType() == Object::Solid_Object)
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

    bool SceneObjectManager::removeObject(Object::Ptr object)
    {
        if(!object)
            return false;

        return removeObjectByLayerType(object, object->getSecondType());
    }

    bool SceneObjectManager::removeObject(sf::String name)
    {
        return removeObject(findObject(name));
    }

    void SceneObjectManager::removeDeadPhysicObject()
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

    void SceneObjectManager::setWorld(b2World* world)
    {
        m_World = world;
    }

}

