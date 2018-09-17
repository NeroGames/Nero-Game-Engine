#ifndef SCENEOBJECTMANAGER_H
#define SCENEOBJECTMANAGER_H

#include <Nero/object/Object.h>
#include <Nero/object/PhysicObject.h>
#include <Nero/object/LayerObject.h>
#include <Box2D/Dynamics/b2World.h>

#include <assert.h>


namespace nero
{
    class ObjectManager
    {
        public:
            typedef std::vector<Object::Ptr>        ObjectTab;
            typedef std::vector<PhysicObject::Ptr>  PhysicObjectTab;
            typedef std::shared_ptr<ObjectManager>  Ptr;

        public:
                            ObjectManager(Object::Ptr root_object, b2World* world);
                           ~ObjectManager();

            //Find by name
            Object::Ptr     findObject(sf::String name);
            Object::Ptr     findLayerObject(sf::String name);
            Object::Ptr     findObjectInLayer(sf::String name, sf::String layer);
            Object::Ptr     findSpriteObject(sf::String name);
            Object::Ptr     findPhysicObject(sf::String name);
            Object::Ptr     findSolidObject(sf::String name);

            //Find by id
            Object::Ptr     findObject(int id);
            Object::Ptr     findLayerObject(int id);
            Object::Ptr     findObjectInLayer(int id, sf::String layer);
            Object::Ptr     findSpriteObject(int id);
            Object::Ptr     findPhysicObject(int id);
            Object::Ptr     findSolidObject(int id);

            //Move
            Object::Ptr     moveObject(sf::String name);
            Object::Ptr     movePhysicObject(sf::String name);
            Object::Ptr     moveSpriteObject(sf::String name);
            Object::Ptr     moveSolidObject(sf::String name);

            //Remove
            bool            removeObject(Object::Ptr object);
            bool            removeObject(sf::String name);

            //
            void            checkAllObject(std::vector<sf::String> objectNameTab);
            //
            void            removeDeadPhysicObject();

            void            setWorld(b2World* world);

            //
            void            addSpriteObject(Object::Ptr object);
            void            addPhysicObject(Object::Ptr object);
            void            addSolidObject(Object::Ptr object);
            void            addObject(Object::Ptr object);


        private:

            Object::Ptr     findObject(Object::Ptr object, sf::String name);
            void            findObject(Object::Ptr object, sf::String name, Object::Ptr& result);
            Object::Ptr     findChildObject(Object::Ptr object, sf::String name);
            Object::Ptr     findObjectByLayerType(sf::String name, Object::Type layer_type);

            //
            Object::Ptr     findObject(Object::Ptr object, int id);
            void            findObject(Object::Ptr object, int id, Object::Ptr& result);
            Object::Ptr     findChildObject(Object::Ptr object, int id);
            Object::Ptr     findObjectByLayerType(int id, Object::Type layer_type);

            //
            Object::Ptr     moveObjectByLayerType(sf::String name, Object::Type layer_type);
            Object::Ptr     moveChildObject(Object::Ptr object, sf::String name);

            //
            bool            removeObjectByLayerType(Object::Ptr object, Object::Type layer_type);
            bool            removeChildObject(Object::Ptr object, Object::Ptr child);

        private:
            Object::Ptr         m_RootObject;
            b2World*            m_World;
            PhysicObjectTab     m_DeadPhysicObject;
    };
}
#endif // SCENEOBJECTMANAGER_H
