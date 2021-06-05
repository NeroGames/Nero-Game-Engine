////////////////////////////////////////////////////////////
// ::Project_Name::
// Copyright (c) ::Coypright_Date:: ::Project_Lead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::Scene_Class::.h"
/////////////////////////////////////////////////////////////

namespace ::Namespace::
{
    nero::GameScene::Ptr ::Scene_Class::::createScene(nero::GameScene::Context context) noexcept
    {
          return ::Scene_Class::::Ptr(new ::Scene_Class::(context));
    }

    ::Scene_Class::::::Scene_Class::(nero::GameScene::Context context) : nero::GameScene(context)
    {

    }

    ::Scene_Class::::~::Scene_Class::()
    {

    }

    void ::Scene_Class::::init()
    {

    }

    void ::Scene_Class::::destroy()
    {

    }

    void ::Scene_Class::::handleEvent(const sf::Event& event)
    {
        nero::GameScene::handleEvent(event);
    }

    void ::Scene_Class::::update(const sf::Time& timeStep)
    {
        nero::GameScene::update(timeStep);
    }

    void  ::Scene_Class::::render()
    {
        nero::GameScene::render();
    }
}
