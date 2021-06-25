////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::ClassName::.h"
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    ::ClassName::::::ClassName::(nero::GameLevel::Context context) : nero::GameLevel(context)
    {

    }

    ::ClassName::::~::ClassName::()
    {

    }

    ::ClassName::::init()
    {

    }

    nero::GameLevel::Ptr ::ClassName::::create::ClassName::(nero::GameLevel::Context context) noexcept
    {
        return ::ClassName::::Ptr(new ::ClassName::(context));
    }


    void ::ClassName::::handleEvent(const sf::Event& event)
    {
        nero::GameLevel::handleEvent(event);
    }

    void ::ClassName::::update(const sf::Time& timeStep)
    {
        nero::GameLevel::update(timeStep);
    }

    void  ::ClassName::::render()
    {
        nero::GameLevel::render();
    }
}
