////////////////////////////////////////////////////////////
// ::ProjectName::
// Copyright (c) ::CoyprightDate:: ::ProjectLead::
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
#include "::SceneClass::.h"
/////////////////////////////////////////////////////////////
namespace ::Namespace::
{
    nero::GameScene::Ptr ::SceneClass::::createScene(nero::GameScene::Context context) noexcept
    {
          return ::SceneClass::::Ptr(new ::SceneClass::(context));
    }

    ::SceneClass::::::SceneClass::(nero::GameScene::Context context) : nero::GameScene(context)
    {

    }

    ::SceneClass::::~::SceneClass::()
    {

    }

    void ::SceneClass::::init()
    {

    }

    void ::SceneClass::::destroy()
    {

    }

    void ::SceneClass::::handleEvent(const sf::Event& event)
    {
        nero::GameScene::handleEvent(event);
    }

    void ::SceneClass::::update(const sf::Time& timeStep)
    {
        nero::GameScene::update(timeStep);
    }

    void  ::SceneClass::::render()
    {
        nero::GameScene::render();
    }
}
