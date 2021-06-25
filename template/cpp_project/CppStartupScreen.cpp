////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::StartupScreenClass::.h"
//Nero
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    ::StartupScreenClass::::::StartupScreenClass::(): nero::StartupScreen()
    {

    }

    void ::StartupScreenClass::::init()
    {

    }

    void ::StartupScreenClass::::handleEvent(sf::Event& event)
    {
        //handle events
    }

    void ::StartupScreenClass::::update(const sf::Time& timeStep)
    {
        //update
    }

    void ::StartupScreenClass::::render()
    {
         //render
    }

    const sf::Color ::StartupScreenClass::::getCanvasColor() const
    {
        return sf::Color::White;
    }

    float ::StartupScreenClass::::getDuration() const
    {
        return 10.f; //seconds
    }
}
