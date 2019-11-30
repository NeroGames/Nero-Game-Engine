////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef FRONTSCREEN_H
#define FRONTSCREEN_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/object/Object.h>
#include <Nero/core/cpp/object/UIObject.h>
/////////////////////////////////////////////////////////////
namespace nero
{
    struct Screen
    {
        typedef std::shared_ptr<Screen>     Ptr;

        Object::Ptr     screen;
        UIObject::Ptr   screenUI;
        std::string     name;
        bool            hide;
        sf::Color       canvasColor;
    };
}
#endif // FRONTSCREEN_H
