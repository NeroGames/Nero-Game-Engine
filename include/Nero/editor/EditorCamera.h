////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORCAMERA_H
#define EDITORCAMERA_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/camera/Camera.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class EditorCamera : public Camera
    {
      public:
        typedef std::shared_ptr<EditorCamera> Ptr;

      public:
        EditorCamera(const sf::Vector2f& defaultPos = sf::Vector2f(0.f, 0.f),
                     const float&        panSpeed   = 300.f,
                     const float&        rotSpeed   = 0.5f,
                     const float&        zRatio     = 0.99f);
        EditorCamera(const sf::Vector2f& viewSize,
                     const sf::Vector2f& defaultPos = sf::Vector2f(0.f, 0.f),
                     const float&        panSpeed   = 300.f,
                     const float&        rotSpeed   = 0.5f,
                     const float&        zRatio     = 0.99f);
        virtual ~EditorCamera();

        void update(const sf::Time& deltaTime);
        void handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
        void handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);
        void cancelAction();

      private:
        bool m_IsPanningUp;
        bool m_IsPanningDown;
        bool m_IsPanningLeft;
        bool m_IsPanningRight;
        bool m_IsRotatingLeft;
        bool m_IsRotatingRight;
        bool m_IsZoomingIn;
        bool m_IsZoomingOut;
    };
} // namespace nero
#endif // EditorCamera_H
