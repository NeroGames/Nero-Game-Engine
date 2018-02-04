////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/camera/DevCamera.h>
////////////////////////////////////////////////////////////

namespace nero
{
    ////////////////////////////////////////////////////////////
    DevCamera::DevCamera(const sf::Vector2f& size, const sf::Vector2f& defaultPos, const float& panSpeed, const float& rotSpeed, const float& zRatio):
            Camera(size, defaultPos, panSpeed, rotSpeed, zRatio),
            m_IsPanningDown(false),
            m_IsPanningLeft(false),
            m_IsPanningRight(false),
            m_IsPanningUp(false),
            //Rotation
            m_IsRotatingLeft(false),
            m_IsRotatingRight(false),
            //Zooming
            m_IsZoomingIn(false),
            m_IsZoomingOut(false)
    {

    }

    ////////////////////////////////////////////////////////////
    DevCamera::~DevCamera()
    {
        //dtor
    }

    ////////////////////////////////////////////////////////////
    void DevCamera::update(const sf::Time& deltaTime)
    {
        //panning
        if(m_IsPanningUp)
            panUp();

        if(m_IsPanningDown)
            panDown();

        if(m_IsPanningLeft)
            panLeft();

        if(m_IsPanningRight)
            panRight();

        //Rotation
        if(m_IsRotatingLeft)
        {
            rotateLeft();
        }

        if(m_IsRotatingRight)
        {
           rotateRight();
        }

        if(m_IsZoomingIn)
        {
            zoomIn();
        }

        if(m_IsZoomingOut)
        {
           zoomOut();
        }

       Camera::update(deltaTime);
    }

    ////////////////////////////////////////////////////////////
    void DevCamera::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Handle key pressing
        if(isPressed)
        {
            //panning
            if(key == sf::Keyboard::Numpad8 && !CTRL_SHIFT_ALT())
                m_IsPanningUp = isPressed;

            if(key == sf::Keyboard::Numpad2 && !CTRL_SHIFT_ALT())
                m_IsPanningDown = isPressed;

            if(key == sf::Keyboard::Numpad4 && !CTRL_SHIFT_ALT())
                m_IsPanningLeft = isPressed;

            if(key == sf::Keyboard::Numpad6 && !CTRL_SHIFT_ALT())
                m_IsPanningRight = isPressed;

            //Roation
            if(key == sf::Keyboard::Numpad7 && !CTRL_SHIFT_ALT())
                m_IsRotatingLeft = isPressed;

            else if(key == sf::Keyboard::Numpad9 && !CTRL_SHIFT_ALT())
                m_IsRotatingRight = isPressed;

            //Zooming
            if(key == sf::Keyboard::Add && !CTRL_SHIFT_ALT())
                m_IsZoomingIn = isPressed;

            else if(key == sf::Keyboard::Subtract && !CTRL_SHIFT_ALT())
                m_IsZoomingOut = isPressed;
        }

        else //Handle key realising
        {
            if(key == sf::Keyboard::Numpad8)
                m_IsPanningUp = isPressed;

            else if(key == sf::Keyboard::Numpad2)
                m_IsPanningDown = isPressed;

            else if(key == sf::Keyboard::Numpad4)
                m_IsPanningLeft = isPressed;

            else if(key == sf::Keyboard::Numpad6)
                m_IsPanningRight = isPressed;

            //Roation
            else if(key == sf::Keyboard::Numpad7)
                m_IsRotatingLeft = isPressed;

            else if(key == sf::Keyboard::Numpad9)
                m_IsRotatingRight = isPressed;

            //Zooming
            else if(key == sf::Keyboard::Add)
                m_IsZoomingIn = isPressed;

            else if(key == sf::Keyboard::Subtract)
                m_IsZoomingOut = isPressed;
        }

    }

    void DevCamera::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
    {
        if (mouse.wheel == sf::Mouse::VerticalWheel)
        {
            if(!CTRL_SHIFT_ALT())
            {
                if(mouse.delta > 0.f)
                {
                    zoomIn();
                    zoomIn();
                    zoomIn();
                }
                else
                {
                    zoomOut();
                    zoomOut();
                    zoomOut();
                }
            }
            else if (CTRL())
            {
                if(mouse.delta > 0.f)
                {
                    panUp();
                    panUp();
                    panUp();
                }
                else
                {
                    panDown();
                    panDown();
                    panDown();
                }
            }
            else if (ALT())
            {
                if(mouse.delta > 0.f)
                {
                    panRight();
                    panRight();
                    panRight();
                }
                else
                {
                    panLeft();
                    panLeft();
                    panLeft();
                }
            }

             else if (SHIFT())
            {
                if(mouse.delta > 0.f)
                {
                    rotateLeft();
                    rotateLeft();
                    rotateLeft();
                }
                else
                {
                    rotateRight();
                    rotateRight();
                    rotateRight();
                }
            }
        }
        else if(mouse.wheel == sf::Mouse::HorizontalWheel)
        {
            if(!CTRL_SHIFT_ALT())
            {
                if(mouse.delta > 0.f)
                {
                    panRight();
                    panRight();
                    panRight();
                }
                else
                {
                    panLeft();
                    panLeft();
                    panLeft();
                }
            }
        }
    }
}
