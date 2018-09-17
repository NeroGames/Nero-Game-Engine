////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/SceneManager.h>
#include <Nero/utility/Utility.h>
//STD
#include <typeinfo>
////////////////////////////////////////////////////////////

namespace nero
{
    SceneManager::SceneManager(Context context):
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

    void SceneManager::update(const sf::Time& timeStep)
    {
        if(!m_AdvancedScene->m_Scene)
            return;

        m_AdvancedScene->update(timeStep);

        if(m_IsShiftOriginLeft && m_IsLeftShift)
        {
            m_AdvancedScene->shiftOrigin(b2Vec2(m_ShitftOriginSpeed, 0.0f));
        }
        if(m_IsShiftOriginRight && m_IsLeftShift)
        {
            m_AdvancedScene->shiftOrigin(b2Vec2(-m_ShitftOriginSpeed, 0.0f));
        }
        if(m_IsShiftOriginUp && m_IsLeftShift)
        {
            m_AdvancedScene->shiftOrigin(b2Vec2(0.0f, m_ShitftOriginSpeed));
        }
        if(m_IsShiftOriginDown && m_IsLeftShift)
        {
            m_AdvancedScene->shiftOrigin(b2Vec2(0.0f, -m_ShitftOriginSpeed));
        }
    }

    void SceneManager::render()
    {
        if(m_AdvancedScene->m_Scene)
        {
            m_AdvancedScene->m_Scene->render();
            m_AdvancedScene->m_Scene->renderShape();
            m_AdvancedScene->renderDebug();

        }

    }

    void SceneManager::renderOnFrontScreen()
    {
        if(m_AdvancedScene->m_Scene)
            m_AdvancedScene->m_Scene->renderFrontScreen();
    }

    void SceneManager::handleEvent(sf::Event& event)
    {
        if(!m_AdvancedScene->m_Scene)
            return;

        m_AdvancedScene->m_Scene->handleEvent(event);

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
                m_AdvancedScene->launchBomb();
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
                m_AdvancedScene->shiftMouseDown(p);
            else
                m_AdvancedScene->mouseDown(p);
        }
        else if (mouse.button == sf::Mouse::Left && isPressed == false)
        {
            m_AdvancedScene->mouseUp(p);
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

        m_AdvancedScene->mouseMove(p);

        if(m_IsMouseRightButton)
        {
            b2Vec2 diff = p - m_LastMousePosition;
            m_AdvancedScene->shiftOrigin(-diff);
            m_LastMousePosition = p;
        }
    }

    std::vector<sf::String> SceneManager::getSceneTable()
    {
        return  m_SceneTable;
    }

    int SceneManager::getSceneCount()
    {
        return m_SceneTable.size();
    }

    void SceneManager::setScene(std::string name)
    {
        auto found = m_SceneFactoryMap.find(name);

        m_AdvancedScene =  found->second.first;
    }

    void SceneManager::restartScene()
    {
//        auto found = m_SceneFactoryMap.find(m_SceneName);
//
//        m_AdvancedScene =  found->second.second();
    }

    b2World*  SceneManager::getPhysicWorld() const
    {
        return m_AdvancedScene->m_Scene->m_PhysicWorld;
    }

    SceneBuilder::Ptr SceneManager::getSceneBuilder()
    {
        return m_AdvancedScene->getSceneBuilder();
    }

    SceneBuilder::Ptr SceneManager::getSceneBuilder(std::string name)
    {
        auto found = m_SceneFactoryMap.find(name);

        return found->second.first->getSceneBuilder();
    }

    UndoManager::Ptr SceneManager::getUndoManager()
    {
        return m_AdvancedScene->getUndoManager();
    }

    SoundManager::Ptr SceneManager::getSoundManager()
    {
        return nullptr;
    }

    UndoManager::Ptr SceneManager::getUndoManager(std::string name)
    {
        auto found = m_SceneFactoryMap.find(name);

        return found->second.first->getUndoManager();
    }

    std::string SceneManager::getSceneName()
    {
        return m_AdvancedScene->m_SceneName;
    }

    MeshEditor::Ptr SceneManager::getMeshEditor()
    {
        return m_AdvancedScene->m_SceneBuilder->getMeshEditor();
    }

    Object::Ptr SceneManager::getRootObject()
    {
        return m_AdvancedScene->m_Scene->m_RootObject;
    }

    void SceneManager::buildScene()
    {
        if(m_AdvancedScene->m_Scene)
        {
            m_AdvancedScene->m_SceneBuilder->destroyAllPhysicObject(m_AdvancedScene->m_Scene->m_RootObject);
            m_AdvancedScene->m_Scene->m_RootObject->removeAllChild();
            m_AdvancedScene->m_Scene = nullptr;
        }

        m_AdvancedScene->m_Scene = m_SceneFactoryMap[m_AdvancedScene->m_SceneName].second();
        m_AdvancedScene->init();
        m_AdvancedScene->m_SceneBuilder->setPhysicWorld(m_AdvancedScene->m_Scene->m_PhysicWorld);
        m_AdvancedScene->m_SceneBuilder->buildScene(m_AdvancedScene->m_Scene->m_RootObject);
        m_AdvancedScene->m_Scene->init();
    }

    CameraSetting& SceneManager::getCameraSetting()
    {
       return m_AdvancedScene->m_CameraSetting;
    }

    SceneSetting& SceneManager::getSceneSetting()
    {
         return m_AdvancedScene->m_SceneSetting;
    }



}
