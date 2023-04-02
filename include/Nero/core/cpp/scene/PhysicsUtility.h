////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICSUTILITY_H
#define PHYSICSUTILITY_H
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////
namespace nero
{
    struct PhysicsPoperty
    {
        bool  fixedRotation;
        bool  sensor;
        bool  allowSleep;
        float density;
        float friction;
        float restitution;
        float gravityScale;
    };

} // namespace nero
#endif // PHYSICSUTILITY_H
