////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef JOINTPROPERTY_H_INCLUDED
#define JOINTPROPERTY_H_INCLUDED
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    struct DistanceJointProperty
    {
        std::string     name                = "distance_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           length              = 50.f;
        float           frequencyHz         = 5.f;
        float           dampingRatio        = 0.5f;
    };

    ////////////////////////////////////////////////////////////
    struct FrictionJointProperty
    {
        std::string     name                = "friction_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           maxForce            = 1.f;
        float           maxTorque           = 1.f;
    };

    ////////////////////////////////////////////////////////////
    struct GearJointProperty
    {
        std::string     name                = "gear_joint";
        bool            collideConnected    = false;
        int             jointAId            = -1;
        int             jointBId            = -1;
        float           ratio               = 1.f;
    };

    ////////////////////////////////////////////////////////////
    struct MotorJointProperty
    {
        std::string     name                = "motor_joint";
        bool            collideConnected    = false;
        sf::Vector2f    linearOffset        = sf::Vector2f(0.f, 0.f);
        float           angularOffset       = 1.f;
        float           maxForce            = 1.f;
        float           maxTorque           = 1.f;
        float           correctionFactor    = 0.5f;
    };

    ////////////////////////////////////////////////////////////
    struct MouseJointProperty
    {
        std::string     name                = "mouse_joint";
        bool            collideConnected    = false;
        sf::Vector2f    target              = sf::Vector2f(0.f, 0.f);
        float           maxForce            = 10.f;
        float           frequencyHz         = 5.f;
        float           dampingRatio        = 0.5f;
    };

    ////////////////////////////////////////////////////////////
    struct PrismaticJointProperty
    {
        std::string     name                = "prismatic_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAxisA          = sf::Vector2f(0.f, 0.f);
        float           referenceAngle      = 0.f;
        bool            enableLimit         = false;
        float           lowerTranslation    = 0.f;
        float           upperTranslation    = 0.f;
        bool            enableMotor         = false;
        float           maxMotorTorque      = 10.f;
        float           motorSpeed          = 0.f;
    };

    ////////////////////////////////////////////////////////////
    struct PulleyJointProperty
    {
        std::string     name                = "pulley_joint";
        bool            collideConnected    = false;
        sf::Vector2f    groundAnchorA       = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    groundAnchorB       = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           lengthA             = 50.f;
        float           lengthB             = 50.f;
        float           ratio               = 1.f;
    };

    ////////////////////////////////////////////////////////////
    struct RevoluteJointProperty
    {
        std::string     name                = "revolute_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           referenceAngle      = 0.f;
        bool            enableLimit         = false;
        float           lowerAngle          = 0.f;
        float           upperAngle          = 0.f;
        bool            enableMotor         = false;
        float           maxMotorTorque       = 10.f;
        float           motorSpeed          = 0.f;
    };

    ////////////////////////////////////////////////////////////
    struct RopeJointProperty
    {
        std::string     name                = "rope_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           maxLength           = 50.f;
    };

    ////////////////////////////////////////////////////////////
    struct WeldJointProperty
    {
        std::string     name                = "weld_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        float           referenceAngle      = 0.f;
        float           frequencyHz         = 5.f;
        float           dampingRatio        = 0.5f;
    };

    ////////////////////////////////////////////////////////////
    struct WheelJointProperty
    {
        std::string     name                = "wheel_joint";
        bool            collideConnected    = false;
        sf::Vector2f    localAnchorA        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAnchorB        = sf::Vector2f(0.f, 0.f);
        sf::Vector2f    localAxisA          = sf::Vector2f(0.f, 0.f);
        float           referenceAngle      = 0.f;
        bool            enableMotor         = false;
        float           maxMotorTorque      = 10.f;
        float           motorSpeed          = 0.f;
        float           frequencyHz         = 5.f;
        float           dampingRatio        = 0.5f;
    };

}
#endif // JOINTPROPERTY_H_INCLUDED
