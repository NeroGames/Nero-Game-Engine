////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICACTIONOBJECT_H
#define PHYSICACTIONOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/PhysicObject.h>
#include <Nero/model/Action.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicActionObject : public Object
    {
       public:
            PhysicActionObject();

            template<class T>
            void        registerAction(sf::String name);
            template<class T>
            void        registerAction(sf::String name, const T& action);


            void                dropAction(sf::String name);
            void                callAction(sf::String name);
            void                setObject(Object::Ptr object);
            PhysicObject::Ptr   getObject();

        private:
            virtual void        updateObject(sf::Time time_step);

        protected:
            PhysicObject::Ptr                           m_PhysicObject;
            sf::Time                                    m_TimeStep;
            std::map<sf::String, PhysicAction::Ptr>     m_ActionTable;
    };

    template <typename T>
    void PhysicActionObject::registerAction(sf::String name)
    {
        m_ActionTable[name] = PhysicAction::Ptr(new T());
    }

    template<class T>
    void PhysicActionObject::registerAction(sf::String name, const T& action)
    {
        m_ActionTable[name] = PhysicAction::Ptr(new T(action));
    }
}
#endif // PHYSICACTIONOBJECT_H
