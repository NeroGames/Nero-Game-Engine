////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef DEVCAMERA_H
#define DEVCAMERA_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/camera/Camera.h>
//SFGUI
#include <SFGUI/Label.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    class DevCamera : public Camera
    {
        public:
                            DevCamera(const sf::Vector2f& size, const sf::Vector2f& defaultPos = sf::Vector2f(0.f, 0.f), const float& panSpeed = 300.f, const float& rotSpeed = 0.5f, const float& zRatio = 0.99f);
            virtual        ~DevCamera();

            void            update(const sf::Time& deltaTime);

            void            handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void            handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);


        private:
            bool            m_IsPanningUp;
            bool            m_IsPanningDown;
            bool            m_IsPanningLeft;
            bool            m_IsPanningRight;

            bool            m_IsRotatingLeft;
            bool            m_IsRotatingRight;

            bool            m_IsZoomingIn;
            bool            m_IsZoomingOut;
    };
}

#endif // DEVCAMERA_H
