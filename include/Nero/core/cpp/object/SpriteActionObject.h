////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef SPRITEACTIONOBJECT_H
#define SPRITEACTIONOBJECT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/SpriteObject.h>
#include <Nero/core/cpp/model/Action.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class SpriteActionObject : public Object
    {
      public:
        SpriteActionObject();

        template <class T> void registerAction(sf::String name);
        template <class T> void registerAction(sf::String name, const T& action);

        void                    dropAction(sf::String name);
        void                    callAction(sf::String name);
        void                    setObject(Object::Ptr object);
        SpriteObject::Ptr       getObject();

      private:
        virtual void updateObject(sf::Time time_step);

      private:
        SpriteObject::Ptr                       m_SpriteObject;
        sf::Time                                m_TimeStep;
        std::map<sf::String, SpriteAction::Ptr> m_ActionTable;
    };

    template <typename T> void SpriteActionObject::registerAction(sf::String name)
    {
        m_ActionTable[name] = SpriteAction::Ptr(new T());
    }

    template <class T> void SpriteActionObject::registerAction(sf::String name, const T& action)
    {
        m_ActionTable[name] = SpriteAction::Ptr(new T(action));
    }
} // namespace nero
#endif // SPRITEACTIONOBJECT_H
