#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <Nero/object/Object.h>
#include <SFGUI/Canvas.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

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
