////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef LAYERACTIONOBJECT_H
#define LAYERACTIONOBJECT_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/LayerObject.h>
#include <Nero/core/cpp/model/Action.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    class LayerActionObject : public Object {
      public:
        LayerActionObject();

        template <class T>
        void registerAction(sf::String name);
        template <class T>
        void             registerAction(sf::String name, const T& action);

        void             dropAction(sf::String name);
        void             callAction(sf::String name);
        void             setObject(Object::Ptr object);
        LayerObject::Ptr getObject();

      private:
        virtual void updateObject(sf::Time time_step);

      private:
        LayerObject::Ptr                       m_LayerObject;
        sf::Time                               m_TimeStep;
        std::map<sf::String, LayerAction::Ptr> m_ActionTable;
    };

    template <typename T>
    void LayerActionObject::registerAction(sf::String name)
    {
        m_ActionTable[name] = LayerAction::Ptr(new T());
    }

    template <class T>
    void LayerActionObject::registerAction(sf::String name, const T& action)
    {
        m_ActionTable[name] = LayerAction::Ptr(new T(action));
    }
} // namespace nero
#endif // LAYERACTIONOBJECT_H
