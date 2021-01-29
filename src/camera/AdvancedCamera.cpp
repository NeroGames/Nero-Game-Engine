////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/camera/AdvancedCamera.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    AdvancedCamera::AdvancedCamera(const sf::Vector2f& viewSize, sfg::Canvas::Ptr renderCanvas, const sf::Vector2f& defaultPos, const float& panSpeed, const float& rotSpeed, const float& zRatio):
         Camera(viewSize, defaultPos, panSpeed, rotSpeed, zRatio)
        ,m_RenderCanvas(renderCanvas)
        ,m_IsPanningDown(false)
        ,m_IsPanningLeft(false)
        ,m_IsPanningRight(false)
        ,m_IsPanningUp(false)
        ,m_IsRotatingLeft(false)
        ,m_IsRotatingRight(false)
        ,m_IsZoomingIn(false)
        ,m_IsZoomingOut(false)
    {
        m_xAxis.setSize(sf::Vector2f(20.f, -2.f));
        m_xAxis.setFillColor(sf::Color::Red);
        m_xAxis.setPosition(sf::Vector2f(20.f, 20.f));

        m_yAxis.setSize(sf::Vector2f(20.f, -2.f));
        m_yAxis.setFillColor(sf::Color::Green);
        m_yAxis.setPosition(sf::Vector2f(20.f, 20.f));
        m_yAxis.setRotation(90.f);
    }

    ////////////////////////////////////////////////////////////
    AdvancedCamera::~AdvancedCamera()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void AdvancedCamera::update(const sf::Time& deltaTime)
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

       m_xAxis.setRotation(-m_View.getRotation());
       m_yAxis.setRotation(-m_View.getRotation() + 90.f);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedCamera::render()
    {
        m_RenderCanvas->Draw(m_xAxis);
        m_RenderCanvas->Draw(m_yAxis);
    }

    ////////////////////////////////////////////////////////////
    void AdvancedCamera::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
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

    ////////////////////////////////////////////////////////////
    void AdvancedCamera::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
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
            else if (ALT())
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
            else if (CTRL())
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

    ////////////////////////////////////////////////////////////
    void AdvancedCamera::cancelAction()
    {
        m_IsPanningUp       = false;
        m_IsPanningDown     = false;
        m_IsPanningLeft     = false;
        m_IsPanningRight    = false;
        m_IsRotatingLeft    = false;
        m_IsRotatingRight   = false;
        m_IsZoomingIn       = false;
        m_IsZoomingOut      = false;
    }
}
