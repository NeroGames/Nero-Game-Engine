////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef CAMERA_H
#define CAMERA_H
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/utility/Utility.h>
// SFML
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/View.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class Camera
    {
      public:
        typedef std::shared_ptr<Camera> Ptr;

      public:
        Camera(const sf::Vector2f& defaultPos = sf::Vector2f(0.f, 0.f),
               const float&        panSpeed   = 300.f,
               const float&        rotSpeed   = 0.5f,
               const float&        zRatio     = 0.99f);
        Camera(const sf::Vector2f& viewSize,
               const sf::Vector2f& defaultPos = sf::Vector2f(0.f, 0.f),
               const float&        panSpeed   = 300.f,
               const float&        rotSpeed   = 0.5f,
               const float&        zRatio     = 0.99f);
        virtual ~Camera();

        virtual void handleEvent(const sf::Event& event);
        virtual void update(const sf::Time& timeStep);
        virtual void render();
        // Handle inputs
        virtual void handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
        virtual void handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                             const bool&                        isPressed);
        virtual void handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
        virtual void handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);
        // Set Settings
        void         setDefaultPosition(const sf::Vector2f& pos, bool update = true);
        void         setDefaultRotation(const float& degree, bool update = true);
        void         setDefaultZoom(const int& nTime, bool update = true);
        void         setPanningSpeed(const float& panSpeed);
        void         setRotationSpeed(const float& rotSpeed);
        void         setZoomingRatio(const float& zRatio);
        // Camera actions
        void         panUp();
        void         panDown();
        void         panLeft();
        void         panRight();
        void         setPosition(const sf::Vector2f& position);
        void         rotateLeft();
        void         rotateRight();
        void         setRotation(const float& degree);
        void         zoomIn();
        void         zoomOut();
        void         setZoom(const int& zoom);
        void         reinitialize();
        sf::Vector2f getPosition() const;
        float        getRotation() const;
        int          getZoom() const;
        sf::View     getView() const;
        //
        void         updateView(const sf::Vector2f& size);

      protected:
        sf::View     m_View;
        sf::Vector2f m_DefaultPosition;
        int          m_DefaultZoom;
        float        m_DefaultRotation;
        sf::Vector2f m_Pan;
        int          m_Zoom;
        float        m_Rotation;
        float        m_PanningSpeed;
        float        m_DefaultPanningSpeed;
        float        m_RotationSpeed;
        float        m_ZoomingRatio;
    };
} // namespace nero
#endif // CAMERA_H
