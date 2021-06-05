////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/ButtonObject.h>
#include <Nero/core/cpp/utility/Utility.h>

////////////////////////////////////////////////////////////
namespace nero
{
   ButtonObject::ButtonObject():
        UIObject()
       ,m_MouseEnter(false)
    {
        setSecondType(Object::Button_Object);

        m_OnMouseClick = [](){};
        m_OnMouseEnter = [](){};
        m_OnMouseLeave = [](){};
    }

    void ButtonObject::onSelect()
    {
        if(m_OnMouseEnter)
            m_OnMouseEnter();
    }

    void ButtonObject::onUnSelect()
    {
        if(m_OnMouseLeave)
            m_OnMouseLeave();
    }

    void ButtonObject::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        if(isPressed && getGlobalBounds().contains(sf::Vector2f(mouse.x, mouse.y)))
        {
            m_OnMouseClick();
        }
    }

    void  ButtonObject::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f position =  sf::Vector2f(mouse.x, mouse.y);

        if(getGlobalBounds().contains(position) && !m_MouseEnter)
        {
            m_MouseEnter = true;
            getFirstChild()->setColor(sf::Color(255, 255, 255, 255));

            if(m_OnMouseEnter)
                m_OnMouseEnter();

        }
        else if(!getGlobalBounds().contains(position) && m_MouseEnter)
        {
            m_MouseEnter = false;
            getFirstChild()->setColor(sf::Color(255, 255, 255, 150));

            if(m_OnMouseLeave)
                m_OnMouseLeave();
        }
    }

    sf::FloatRect ButtonObject::getGlobalBounds() const
    {
        return getFirstChild()->getGlobalBounds();
    }

    ButtonObject::Ptr ButtonObject::Cast(Object::Ptr object)
    {
        if(object->getSecondType() != Object::Button_Object)
            return nullptr;

        return  std::static_pointer_cast<ButtonObject>(object);
    }

    void ButtonObject::setOnMouseEnter(std::function<void()> callback)
    {
        m_OnMouseEnter = callback;
    }

    void ButtonObject::setOnMouseLeave(std::function<void ()> callback)
    {
        m_OnMouseLeave = callback;
    }

    void ButtonObject::setOnMouseClick(std::function<void ()> callback)
    {
        m_OnMouseClick = callback;
    }
}

