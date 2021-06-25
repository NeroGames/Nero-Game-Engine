////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::GameLevelClass::.h"
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    nero::GameLevel::Ptr ::GameLevelClass::::create::GameLevelClass::(nero::GameLevel::Context context) noexcept
    {
        return ::GameLevelClass::::Ptr(new ::GameLevelClass::(context));
    }

    ::GameLevelClass::::::GameLevelClass::(nero::GameLevel::Context context) : nero::GameLevel(context)
    {

    }

    ::GameLevelClass::::~::GameLevelClass::()
    {

    }

    void ::GameLevelClass::::init()
    {

    }

    void ::GameLevelClass::::handleEvent(const sf::Event& event)
    {
        nero::GameLevel::handleEvent(event);
    }

    void ::GameLevelClass::::update(const sf::Time& timeStep)
    {
        nero::GameLevel::update(timeStep);
    }

    void ::GameLevelClass::::render()
    {
        nero::GameLevel::render();
    }
}
