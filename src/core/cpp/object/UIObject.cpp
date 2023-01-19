////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/object/UIObject.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    UIObject::UIObject()
        : Object()
    {
        setFirstType(Object::UI_Object);
        setSecondType(Object::UI_Object);
    }

    void UIObject::handleEvent(const sf::Event& event)
    {
        if(getFirstType() != Object::UI_Object)
            return;

        handleObjectEvent(event);
        handleAllChildEvent(event);
    }

    void UIObject::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse,
                                          const bool&                        isPressed)
    {
        // Empty
    }

    void UIObject::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        // Empty
    }

    void UIObject::handleObjectEvent(const sf::Event& event)
    {
        switch(event.type)
        {
        // Mouse Button
        case sf::Event::MouseButtonPressed:
            handleMouseButtonInput(event.mouseButton, true);
            break;
        case sf::Event::MouseButtonReleased:
            handleMouseButtonInput(event.mouseButton, false);
            break;

        // Mouse move
        case sf::Event::MouseMoved:
            handleMouseMoveInput(event.mouseMove);
            break;
        }
    }

    void UIObject::handleAllChildEvent(const sf::Event& event)
    {
        for(Object::Ptr child : m_ChildTab)
        {
            if(child->getFirstType() == Object::UI_Object)
            {
                UIObject::Cast(child)->handleEvent(event);
            }
        }
    }

    UIObject::Ptr UIObject::Cast(Object::Ptr object)
    {
        if(object->getFirstType() != Object::UI_Object)
            return nullptr;

        return std::static_pointer_cast<UIObject>(object);
    }
} // namespace nero
