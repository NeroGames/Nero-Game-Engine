////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::Class_Name::.h"
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    ::Class_Name::::::Class_Name::(nero::GameScreen::Context context) : nero::GameScreen(context)
    {

    }

    ::Class_Name::::~::Class_Name::()
    {

    }

    ::Class_Name::::init()
    {

    }

    nero::GameScreen::Ptr ::Class_Name::::create::Class_Name::(nero::GameScreen::Context context) noexcept
    {
        return ::Class_Name::::Ptr(new ::Class_Name::(context));
    }

    void ::Class_Name::::handleEvent(const sf::Event& event)
    {
        nero::GameScreen::handleEvent(event);
    }

    void ::Class_Name::::update(const sf::Time& timeStep)
    {
        nero::GameScreen::update(timeStep);
    }

    void ::Class_Name::::render()
    {
        nero::GameScreen::render();
    }
}
