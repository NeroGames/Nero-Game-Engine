////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef BUTTONOBJECT_H
#define BUTTONOBJECT_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/UIObject.h>
//STD
#include <functional>
/////////////////////////////////////////////////////////////
namespace nero
{
    class ButtonObject : public UIObject
    {
        public:
            typedef std::shared_ptr<ButtonObject>     Ptr;
            static ButtonObject::Ptr                  Cast(Object::Ptr object);

                                        ButtonObject();

            void                        onSelect();
            void                        onUnSelect();

            void                        setOnMouseEnter(std::function<void()> callback);
            void                        setOnMouseLeave(std::function<void()> callback);
            void                        setOnMouseClick(std::function<void()> callback);


        private:
            void                        handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                        handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
            virtual sf::FloatRect       getGlobalBounds() const;

        private:
            std::function<void()>       m_OnMouseEnter;
            std::function<void()>       m_OnMouseLeave;
            std::function<void()>       m_OnMouseClick;
            bool                        m_MouseEnter;
    };
}
#endif // BUTTONOBJECT_H
