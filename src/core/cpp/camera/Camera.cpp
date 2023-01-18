////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// NERO
#include <Nero/core/cpp/camera/Camera.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    Camera::Camera(const sf::Vector2f& defaultPos, const float& panSpeed, const float& rotSpeed, const float& zRatio)
        : m_DefaultPosition(defaultPos)
        , m_PanningSpeed(panSpeed)
        , m_DefaultPanningSpeed(panSpeed)
        , m_RotationSpeed(rotSpeed)
        , m_ZoomingRatio(zRatio)
        , m_DefaultZoom(0)
        , m_Pan(0.f, 0.f)
        , m_Rotation(0.f)
        , m_DefaultRotation(0.f)
        , m_Zoom(0)
        , m_View()
    {
        m_View.setCenter(defaultPos);
        m_View.setSize(sf::Vector2f(100.f, 100.f));
    }

    ////////////////////////////////////////////////////////////
    Camera::Camera(const sf::Vector2f& viewSize, const sf::Vector2f& defaultPos, const float& panSpeed, const float& rotSpeed, const float& zRatio)
        : m_DefaultPosition(defaultPos)
        , m_PanningSpeed(panSpeed)
        , m_DefaultPanningSpeed(panSpeed)
        , m_RotationSpeed(rotSpeed)
        , m_ZoomingRatio(zRatio)
        , m_DefaultZoom(0)
        , m_Pan(0.f, 0.f)
        , m_Rotation(0.f)
        , m_DefaultRotation(0.f)
        , m_Zoom(0)
        , m_View()
    {
        m_View.setCenter(defaultPos);
        m_View.setSize(sf::Vector2f(viewSize.x, viewSize.y));
    }

    ////////////////////////////////////////////////////////////
    Camera::~Camera()
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::handleEvent(const sf::Event& event)
    {
        switch(event.type)
        {
        // Keyboard events
        case sf::Event::KeyPressed:
            handleKeyboardInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handleKeyboardInput(event.key.code, false);
            break;

        // Mouse buttons events
        case sf::Event::MouseButtonPressed:
            handleMouseButtonsInput(event.mouseButton, true);
            break;
        case sf::Event::MouseButtonReleased:
            handleMouseButtonsInput(event.mouseButton, false);
            break;

        // Mouse movements event
        case sf::Event::MouseMoved:
            handleMouseMoveInput(event.mouseMove);
            break;

        // Mouse wheel
        case sf::Event::MouseWheelScrolled:
            handleMouseWheelInput(event.mouseWheelScroll);
            break;
        }
    }

    ////////////////////////////////////////////////////////////
    void Camera::update(const sf::Time& timeStep)
    {
        sf::Transform t;
        t.rotate(m_Rotation);
        m_Pan = t * m_Pan;
        m_View.move(m_Pan * timeStep.asSeconds());

        m_Pan = sf::Vector2f(0.f, 0.f);
    }

    ////////////////////////////////////////////////////////////
    void Camera::render()
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
    {
        // Empty
    }

    ////////////////////////////////////////////////////////////
    void Camera::panUp()
    {
        m_Pan.y -= m_PanningSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::panDown()
    {
        m_Pan.y += m_PanningSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::panLeft()
    {
        m_Pan.x -= m_PanningSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::panRight()
    {
        m_Pan.x += m_PanningSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setPosition(const sf::Vector2f& position)
    {
        m_View.setCenter(position);
    }

    ////////////////////////////////////////////////////////////
    void Camera::rotateLeft()
    {
        m_View.rotate(-m_RotationSpeed);
        m_Rotation -= m_RotationSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::rotateRight()
    {
        m_View.rotate(m_RotationSpeed);
        m_Rotation += m_RotationSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setRotation(const float& degree)
    {
        m_View.rotate(degree - m_Rotation);
        m_Rotation += degree - m_Rotation;
    }

    ////////////////////////////////////////////////////////////
    void Camera::zoomIn()
    {
        m_View.zoom(m_ZoomingRatio);
        m_Zoom++;
        (m_PanningSpeed <= 0) ? 1.f : m_PanningSpeed--;
    }

    ////////////////////////////////////////////////////////////
    void Camera::zoomOut()
    {
        m_View.zoom(1 / m_ZoomingRatio);
        m_Zoom--;
        m_PanningSpeed++;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setZoom(const int& zoom)
    {
        if(m_Zoom > zoom)
            m_View.zoom(1 / std::pow(m_ZoomingRatio, (m_Zoom - zoom)));

        if(m_Zoom < zoom)
            m_View.zoom(pow(m_ZoomingRatio, (zoom - m_Zoom)));

        m_Zoom = zoom;
    }

    ////////////////////////////////////////////////////////////
    void Camera::reinitialize()
    {
        //---Reset Position---------------->
        setPosition(m_DefaultPosition);
        //---Reset Zoom-------------------->
        setZoom(m_DefaultZoom);
        //---Reset Rotation-------------->
        setRotation(m_DefaultRotation);
        //---Reset Speed-------------->
        m_PanningSpeed = m_DefaultPanningSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setDefaultPosition(const sf::Vector2f& pos, bool update)
    {
        m_DefaultPosition = pos;

        if(update)
            m_View.setCenter(pos);
    }

    ////////////////////////////////////////////////////////////
    void Camera::setDefaultZoom(const int& zoom, bool update)
    {
        m_DefaultZoom = zoom;

        if(update)
            setZoom(zoom);
    }

    ////////////////////////////////////////////////////////////
    void Camera::setDefaultRotation(const float& degree, bool update)
    {
        m_DefaultRotation = degree;

        if(update)
            setRotation(degree);
    }

    ////////////////////////////////////////////////////////////
    void Camera::setPanningSpeed(const float& panSpeed)
    {
        m_PanningSpeed = panSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setRotationSpeed(const float& rotSpeed)
    {
        m_RotationSpeed = rotSpeed;
    }

    ////////////////////////////////////////////////////////////
    void Camera::setZoomingRatio(const float& zRatio)
    {
        m_ZoomingRatio = zRatio;
    }

    ////////////////////////////////////////////////////////////
    sf::Vector2f Camera::getPosition() const
    {
        return m_View.getCenter();
    }

    ////////////////////////////////////////////////////////////
    float Camera::getRotation() const
    {
        return m_Rotation;
    }

    ////////////////////////////////////////////////////////////
    int Camera::getZoom() const
    {
        return m_Zoom;
    }

    ////////////////////////////////////////////////////////////
    sf::View Camera::getView() const
    {
        return m_View;
    }

    void Camera::updateView(const sf::Vector2f& size)
    {
        m_View.setSize(size);
    }
} // namespace nero
////////////////////////////////////////////////////////////
