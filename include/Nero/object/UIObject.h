////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef UIOBJECT_H
#define UIOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/object/Object.h>
//SFGUI
#include <SFGUI/Canvas.hpp>
//SFML
#include <SFML/Window/Event.hpp>
//STD
#include <functional>
/////////////////////////////////////////////////////////////
namespace nero
{
    class UIObject : public Object
    {
        public:
            typedef std::shared_ptr<UIObject>     Ptr;
            static UIObject::Ptr                  Cast(Object::Ptr object);

                                        UIObject();

            void                        handleEvent(const sf::Event& event);

        protected:
            virtual void                handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            virtual void                handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

            virtual void			    handleObjectEvent(const sf::Event& event);
            void					    handleAllChildEvent(const sf::Event& event);
    };
}
#endif // UIOBJECT_H
