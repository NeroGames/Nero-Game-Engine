////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::Scene_Class::.h"
/////////////////////////////////////////////////////////////

namespace ::Namespace::
{
    nero::Scene::Ptr ::Scene_Class::::createScene(nero::Scene::Context context) noexcept
    {
          return ::Scene_Class::::Ptr(new ::Scene_Class::(context));
    }

    ::Scene_Class::::::Scene_Class::(nero::Scene::Context context) : nero::Scene(context)
    {

    }

    ::Scene_Class::::~::Scene_Class::()
    {

    }

    void ::Scene_Class::::init()
    {

    }

    void ::Scene_Class::::handleEvent(const sf::Event& event)
    {
        nero::Scene::handleEvent(event);
    }

    void ::Scene_Class::::update(const sf::Time& timeStep)
    {
        nero::Scene::update(timeStep);
    }

    void  ::Scene_Class::::render()
    {
        nero::Scene::render();
    }
}
