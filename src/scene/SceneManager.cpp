#include <NERO/scene/SceneManager.h>
#include <NERO/Utility.h>
#include <typeinfo>

//TEst----------
#include <NERO/resource/Resource.h>

namespace nero
{
    SceneManager::SceneManager(Scene::Context context):
    m_Context(context),
    m_IsLeftShift(false),
    m_IsMouseRightButton(false),
    m_IsShiftOriginUp(false),
    m_IsShiftOriginDown(false),
    m_IsShiftOriginLeft(false),
    m_IsShiftOriginRight(false),
    m_ShitftOriginSpeed(0.5f),
    m_ViewCenter(0.0f, 20.0f)
    {
        //
    }

    void SceneManager::update()
    {
        m_SceneSettings.viewCenter = m_ViewCenter;
        m_DevScene->update(&m_SceneSettings);

        if(m_IsShiftOriginLeft && m_IsLeftShift)
        {
            m_DevScene->ShiftOrigin(b2Vec2(m_ShitftOriginSpeed, 0.0f));
        }
        if(m_IsShiftOriginRight && m_IsLeftShift)
        {
            m_DevScene->ShiftOrigin(b2Vec2(-m_ShitftOriginSpeed, 0.0f));
        }
        if(m_IsShiftOriginUp && m_IsLeftShift)
        {
            m_DevScene->ShiftOrigin(b2Vec2(0.0f, m_ShitftOriginSpeed));
        }
        if(m_IsShiftOriginDown && m_IsLeftShift)
        {
            m_DevScene->ShiftOrigin(b2Vec2(0.0f, -m_ShitftOriginSpeed));
        }
    }

    void SceneManager::render()
    {
        m_DevScene->m_Scene->render();
    }

    void SceneManager::renderShape()
    {
        m_DevScene->m_Scene->renderShape();
    }


    void SceneManager::renderDebug()
    {
        m_DevScene->renderDebug();
    }

    void SceneManager::renderOnFrontScreen()
    {
        m_DevScene->m_Scene->renderOnFrontScreen();
    }

    void SceneManager::handleEvent(sf::Event& event)
    {
        m_DevScene->m_Scene->handleEvent(event);

        switch(event.type)
        {
            //Keyboard events
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

            //Mouse buttons events
            case sf::Event::MouseButtonPressed:
                handleMouseButtonsInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonsInput(event.mouseButton, false);
                break;

            //Mouse mouvements event
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;
        }
    }

    void SceneManager::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        //Handle only key pressed inputs
        if(isPressed)
        {
            if(key == sf::Keyboard::B)
                m_DevScene->LaunchBomb();
        }

        if(key == sf::Keyboard::Z)
            m_IsShiftOriginUp = isPressed;

        else if(key == sf::Keyboard::S)
            m_IsShiftOriginDown = isPressed;

        else if(key == sf::Keyboard::Q)
            m_IsShiftOriginLeft = isPressed;

        else if(key == sf::Keyboard::D)
            m_IsShiftOriginRight = isPressed;

        else if(key == sf::Keyboard::LShift)
            m_IsLeftShift = isPressed;
    }

    void SceneManager::handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_Context.renderCanvas);
        b2Vec2 p = sf_to_b2(world_pos, SCALE);

        if(mouse.button == sf::Mouse::Left && isPressed == true)
        {
            if(m_IsLeftShift)
                m_DevScene->ShiftMouseDown(p);
            else
                m_DevScene->MouseDown(p);
        }
        else if (mouse.button == sf::Mouse::Left && isPressed == false)
        {
            m_DevScene->MouseUp(p);
        }
        else if (mouse.button == sf::Mouse::Right)
        {
            if(isPressed)
            {
                m_LastMousePosition = p;

            }

            m_IsMouseRightButton = isPressed;
        }

    }

    void SceneManager::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_Context.renderCanvas);
        b2Vec2 p = sf_to_b2(world_pos, SCALE);

        m_DevScene->MouseMove(p);

        if(m_IsMouseRightButton)
        {
            b2Vec2 diff = p - m_LastMousePosition;
            m_DevScene->ShiftOrigin(-diff);
            m_LastMousePosition = p;
        }
    }

    std::vector<sf::String> SceneManager::getSceneList()
    {
        return  m_SceneList;
    }

    int SceneManager::getSceneCount()
    {
        return m_SceneList.size();
    }

    void SceneManager::setScene(sf::String name)
    {
        if(name != "")
        {
            m_SceneName = name;

            auto found = m_Factories.find(name);

            m_DevScene =  found->second.first;
        }

    }

    void SceneManager::restartScene()
    {
//        auto found = m_Factories.find(m_SceneName);
//
//        m_DevScene =  found->second.second();
    }

    b2World*  SceneManager::getSceneWorld() const
    {
        return m_DevScene->m_Scene->m_World;
    }

    ObjectManager* SceneManager::getObjectManager()
    {
        return m_DevScene->getObjectManager();
    }

    ObjectManager* SceneManager::getObjectManager(std::string name)
    {
        auto found = m_Factories.find(name);

        return found->second.first->getObjectManager();
    }

    UndoManager* SceneManager::getUndoManager()
    {
        return m_DevScene->getUndoManager();
    }

    UndoManager* SceneManager::getUndoManager(std::string name)
    {
        auto found = m_Factories.find(name);

        return found->second.first->getUndoManager();
    }

    std::string SceneManager::getSceneName()
    {
        return m_DevScene->getName();
    }

    void SceneManager::setCameraSettings(CameraSettings cameraSettings)
    {
        m_DevScene->setCameraSettings(cameraSettings);
    }

    CameraSettings& SceneManager::getCameraSetting()
    {
        m_DevScene->getCameraSettings();
    }

    SceneSettings& SceneManager::getSceneSettings()
    {
        return m_SceneSettings;
    }

    MeshEditor* SceneManager::getMeshEditor()
    {
        return m_DevScene->getObjectManager()->getMeshEditor();
    }

    Object::Ptr SceneManager::getRootObject()
    {
        return m_DevScene->m_Scene->m_RootObject;
    }

    void SceneManager::buildScene()
    {
        if(m_DevScene->m_Scene)
        {
            m_DevScene->m_ObjectManager.destroyAllPhysicObject(m_DevScene->m_Scene->m_RootObject);
            m_DevScene->m_Scene->m_RootObject->removeAllChild();
            m_DevScene->m_Scene = nullptr;
        }

        m_DevScene->m_Scene = m_Factories[m_DevScene->m_Name].second();
        m_DevScene->init();
        m_DevScene->m_ObjectManager.setWorld(m_DevScene->m_Scene->m_World);
        m_DevScene->m_ObjectManager.buildScene(m_DevScene->m_Scene->m_RootObject);
        m_DevScene->m_Scene->init();
    }

}
