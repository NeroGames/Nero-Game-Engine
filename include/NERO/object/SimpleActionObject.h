////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef SIMPLEACTIONOBJECT_H
#define SIMPLEACTIONOBJECT_H

#include <map>
#include <Nero/object/Object.h>
#include <Nero/model/Action.h>

namespace nero
{
    class SimpleActionObject : public Object
    {
        public:
            SimpleActionObject();

            template<class T>
            void registerAction(sf::String name);

            template<class T>
            void registerAction(sf::String name, const T& action);


            void                dropAction(sf::String name);
            void                callAction(sf::String name);
            void                setObject(Object::Ptr object);
            Object::Ptr         getObject();

        private:
            virtual void        updateObject(sf::Time time_step);

        private:
            Object::Ptr                             m_Object;
            sf::Time                                m_TimeStep;
            std::map<sf::String, ObjectAction::Ptr> m_ActionTable;
    };

    template <typename T>
    void SimpleActionObject::registerAction(sf::String name)
    {
        m_ActionTable[name] = ObjectAction::Ptr(new T());
    }

    template<class T>
    void SimpleActionObject::registerAction(sf::String name, const T& action)
    {
        m_ActionTable[name] = ObjectAction::Ptr(new T(action));
    }

}
#endif // SIMPLEACTIONOBJECT_H
