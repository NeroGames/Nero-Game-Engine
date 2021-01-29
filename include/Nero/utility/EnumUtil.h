////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ENUMUTIL_H_INCLUDED
#define ENUMUTIL_H_INCLUDED
////////////////////////////////////////////////////////////
namespace nero
{
    //ENUM
    enum EngineMode         {SCENE, HELP};
    enum EngineSubMode      {OBJECT, MESH, PLAY};
    enum GridFace           {H_FRONT, H_BACK, V_FRONT, V_BACK};
    enum Direction          {RIGHT, LEFT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT};


    const std::string GridFaceString[4]   {"h_front", "h_back", "v_front", "v_back"};
    const std::string DirectionString[8]  {"right", "left", "up", "down", "up_right", "up_left", "down_right", "down_left"};
}
#endif // ENUMUTIL_H_INCLUDED

