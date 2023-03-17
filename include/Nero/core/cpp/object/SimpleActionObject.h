////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SIMPLEACTIONOBJECT_H
#define SIMPLEACTIONOBJECT_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/model/Action.h>
// STD
#include <map>
/////////////////////////////////////////////////////////////
namespace nero
{
    class SimpleActionObject : public Object
    {
      public:
        SimpleActionObject();

        template <class T> void registerAction(sf::String name);

        template <class T> void registerAction(sf::String name, const T& action);

        void                    dropAction(sf::String name);
        void                    callAction(sf::String name);
        void                    setObject(Object::Ptr object);
        Object::Ptr             getObject();

      private:
        virtual void updateObject(sf::Time time_step);

      private:
        Object::Ptr                             m_Object;
        sf::Time                                m_TimeStep;
        std::map<sf::String, ObjectAction::Ptr> m_ActionTable;
    };

    template <typename T> void SimpleActionObject::registerAction(sf::String name)
    {
        m_ActionTable[name] = ObjectAction::Ptr(new T());
    }

    template <class T> void SimpleActionObject::registerAction(sf::String name, const T& action)
    {
        m_ActionTable[name] = ObjectAction::Ptr(new T(action));
    }

} // namespace nero
#endif // SIMPLEACTIONOBJECT_H
