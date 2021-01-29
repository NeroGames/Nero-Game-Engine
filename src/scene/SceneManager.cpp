////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
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
    SceneManager::SceneManager(Scene::Context context):
         m_Context(context)
        ,m_IsLeftShift(false)
        ,m_IsMouseRightButton(false)
        ,m_IsShiftOriginUp(false)
        ,m_IsShiftOriginDown(false)
        ,m_IsShiftOriginLeft(false)
        ,m_IsShiftOriginRight(false)
        ,m_ShitftOriginSpeed(0.5f)
        ,m_ViewCenter(0.0f, 20.0f)
    {
        m_UpdateUI      = [](){};
        m_UpdateUndo    = [](){};
        m_UpdateLog     = [](const std::string&, int){};
        m_UpdateLogIf   = [](const std::string&, bool, int){};
    }

    void SceneManager::update(const sf::Time& timeStep)
    {
        if(!m_AdvancedScene->m_Scene)
            return;

        m_AdvancedScene->update(timeStep);
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

    void SceneManager::renderFrontScreen()
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

        if(key == sf::Keyboard::LShift)
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
    }



    const int& SceneManager::getSceneCount() const
    {
        return m_SceneTable.size();
    }

    void SceneManager::setScene(std::string name)
    {
        auto found = m_SceneFactoryMap.find(name);

        m_AdvancedScene =  found->second.first;
    }

    SceneBuilder::Ptr SceneManager::getSceneBuilder()
    {
        return m_AdvancedScene->getSceneBuilder();
    }

    SceneBuilder::Ptr SceneManager::getScreenBuilder()
    {
        return m_AdvancedScene->getScreenBuilder();
    }

    UndoManager::Ptr SceneManager::getUndoManager()
    {
        return m_AdvancedScene->getUndoManager();
    }

    UndoManager::Ptr SceneManager::getScreenUndoManager()
    {
        return m_AdvancedScene->getScreenUndoManager();
    }

    SoundManager::Ptr SceneManager::getSoundManager()
    {
        return  m_AdvancedScene->getSoundManager();;
    }

    MeshEditor::Ptr SceneManager::getMeshEditor()
    {
        return m_AdvancedScene->m_SceneBuilder->getMeshEditor();
    }

    void SceneManager::buildScene()
    {
        if(m_AdvancedScene->m_Scene)
        {
            m_AdvancedScene->m_SceneBuilder->setPhysicWorld(m_AdvancedScene->m_Scene->m_PhysicWorld);
            m_AdvancedScene->m_SceneBuilder->destroyAllPhysicObject(m_AdvancedScene->m_Scene->m_World);
            m_AdvancedScene->destroyBomb();
            m_AdvancedScene->m_Scene->m_World->removeAllChild();
            m_AdvancedScene->m_Scene = nullptr;
        }

        m_AdvancedScene->m_Scene = m_SceneFactoryMap[m_AdvancedScene->m_SceneName].second();
        m_AdvancedScene->m_Scene->m_UpdateLog = m_UpdateLog;
        m_AdvancedScene->m_Scene->m_UpdateLogIf = m_UpdateLogIf;
        m_AdvancedScene->init();

        //World
        m_AdvancedScene->m_SceneBuilder->setPhysicWorld(m_AdvancedScene->m_Scene->m_PhysicWorld);
        m_AdvancedScene->m_SceneBuilder->buildScene(m_AdvancedScene->m_Scene->m_World);
        //Screen
        for(auto& devScreen : m_AdvancedScene->m_FrontScreenTable)
        {
            Screen::Ptr screen = Screen::Ptr(new Screen());

            //FrontScreen
            screen->screen  = Object::Ptr(new Object());
            screen->screenUI = UIObject::Ptr(new UIObject());

            devScreen.screenBuilder->buildScene(screen->screen);
            devScreen.screenBuilder->buildUI(screen->screenUI);

            screen->name = devScreen.name;
            screen->hide = true;
            screen->canvasColor = devScreen.screenBuilder->getCanvasColor();

            m_AdvancedScene->m_Scene->m_ScreenTable.push_back(screen);
        }

        m_AdvancedScene->m_Scene->m_ObjectManager->setObjectCount(m_AdvancedScene->m_SceneBuilder->getObjectCount());
        m_AdvancedScene->m_Scene->m_ObjectManager->setPhysicWorld(m_AdvancedScene->m_Scene->m_PhysicWorld);
        m_AdvancedScene->m_Scene->checkSceneObject();
        m_AdvancedScene->m_Scene->init();
    }

    CameraSetting& SceneManager::getCameraSetting()
    {
       return m_AdvancedScene->m_CameraSetting;
    }

    CameraSetting& SceneManager::getScreenCameraSetting()
    {
       return m_AdvancedScene->getScreenCameraSetting();
    }

    SceneSetting& SceneManager::getSceneSetting()
    {
         return m_AdvancedScene->m_SceneSetting;
    }

    nlohmann::json SceneManager::firstSave(const std::string& sceneName)
    {
        auto found = m_SceneFactoryMap.find(sceneName);
        auto advanceScene = found->second.first = found->second.first;

        advanceScene->setUpdateUI(m_UpdateUI);
        advanceScene->setUpdateUndo(m_UpdateUndo);
        advanceScene->setUpdateLog(m_UpdateLog);
        advanceScene->setUpdateLogIf(m_UpdateLogIf);

        advanceScene->m_CameraSetting.position                                  = sf::Vector2f(400.f, 303.f);
        advanceScene->m_CameraSetting.defaultPosition                           = sf::Vector2f(400.f, 303.f);
        advanceScene->m_CameraSetting.zoom                                      = -36;
        advanceScene->m_FrontScreenTable.front().cameraSetting.position        = sf::Vector2f(400.f, 303.f);
        advanceScene->m_FrontScreenTable.front().cameraSetting.defaultPosition = sf::Vector2f(400.f, 303.f);
        advanceScene->m_FrontScreenTable.front().cameraSetting.zoom            = -36;

        nlohmann::json scene = saveScene(advanceScene);

        //World
        advanceScene->m_SceneBuilder->setUpdateUI(m_UpdateUI);
        advanceScene->m_SceneBuilder->setUpdateUndo(m_UpdateUndo);
        advanceScene->m_SceneBuilder->setUpdateLog(m_UpdateLog);
        advanceScene->m_SceneBuilder->setUpdateLogIf(m_UpdateLogIf);
        advanceScene->m_UndoManager->add(scene["world_view"]);
        //Screen
        advanceScene->m_FrontScreenTable.front().screenBuilder->setUpdateUI(m_UpdateUI);
        advanceScene->m_FrontScreenTable.front().screenBuilder->setUpdateUndo(m_UpdateUndo);
        advanceScene->m_FrontScreenTable.front().screenBuilder->setUpdateLog(m_UpdateLog);
        advanceScene->m_FrontScreenTable.front().screenBuilder->setUpdateLogIf(m_UpdateLogIf);
        advanceScene->m_FrontScreenTable.front().undoManager->add(advanceScene->m_FrontScreenTable.front().screenBuilder->saveScene());

        return scene;
    }


    nlohmann::json SceneManager::saveScene()
    {
        return saveScene(m_AdvancedScene);
    }

    nlohmann::json SceneManager::saveScene(AdvancedScene::Ptr advanceScene)
    {
        nlohmann::json scene;
        //Scene
        scene["scene_name"]             = advanceScene->m_SceneName;
        scene["scene_setting"]          = advanceScene->m_SceneSetting.toJson();
        scene["sound_setting"]          = advanceScene->m_SoundManager->toJson();
        //World
        nlohmann::json world_view       = advanceScene->m_SceneBuilder->saveScene();
        world_view["camera_setting"]    = advanceScene->m_CameraSetting.toJson();
        world_view["grid_setting"]      = advanceScene->m_Grid->toJson();
        //Screen
        std::vector<nlohmann::json> screen_table;
        for(auto screen : advanceScene->m_FrontScreenTable)
        {
            nlohmann::json screen_view      = screen.screenBuilder->saveScene();
            screen_view["camera_setting"]   = screen.cameraSetting.toJson();
            screen_view["grid_setting"]     = screen.grid->toJson();
            screen_view["name"]             = screen.name;

            screen_table.push_back(screen_view);
        }
        //
        scene["world_view"]             = world_view;
        scene["screen_table"]           = screen_table;

        return scene;
    }

    void SceneManager::firstLoad(nlohmann::json scene, const std::string& sceneName)
    {
        auto found = m_SceneFactoryMap.find(sceneName);
        auto advanceScene = found->second.first;

        advanceScene->setUpdateUI(m_UpdateUI);
        advanceScene->setUpdateUndo(m_UpdateUndo);
        advanceScene->setUpdateLog(m_UpdateLog);
        advanceScene->setUpdateLogIf(m_UpdateLogIf);

        //Load Scene
        loadScene(advanceScene, scene);

        //World
        advanceScene->m_SceneBuilder->setUpdateUI(m_UpdateUI);
        advanceScene->m_SceneBuilder->setUpdateUndo(m_UpdateUndo);
        advanceScene->m_SceneBuilder->setUpdateLog(m_UpdateLog);
        advanceScene->m_SceneBuilder->setUpdateLogIf(m_UpdateLogIf);
        advanceScene->m_UndoManager->add(scene["world_view"]);
        //Screen
        for(auto screen : advanceScene->m_FrontScreenTable)
        {
            screen.screenBuilder->setUpdateUI(m_UpdateUI);
            screen.screenBuilder->setUpdateUndo(m_UpdateUndo);
            screen.screenBuilder->setUpdateLog(m_UpdateLog);
            screen.screenBuilder->setUpdateLogIf(m_UpdateLogIf);
            screen.undoManager->add(screen.screenBuilder->saveScene());
        }
    }

    void SceneManager::loadScene(nlohmann::json scene)
    {
        loadScene(m_AdvancedScene, scene);
    }

    void SceneManager::loadScene(AdvancedScene::Ptr advanceScene, nlohmann::json scene)
    {
        //Scene
        advanceScene->m_SceneSetting = SceneSetting::fromJson(scene["scene_setting"]);
        advanceScene->m_SoundManager->fromJson(scene["sound_setting"]);
        //World
        advanceScene->m_SceneBuilder->loadScene(scene["world_view"]);
        advanceScene->m_CameraSetting = CameraSetting::fromJson(scene["world_view"]["camera_setting"]);
        advanceScene->m_Grid->fromJson(scene["world_view"]["grid_setting"]);

        //Screen
        nlohmann::json screen_table = scene["screen_table"];
        advanceScene->m_FrontScreenTable.clear();
        for(auto screen : screen_table)
        {
            advanceScene->loadScreen(screen["name"]);
            advanceScene->m_FrontScreenTable.back().screenBuilder->loadScene(screen);
            advanceScene->m_FrontScreenTable.back().cameraSetting = CameraSetting::fromJson(screen["camera_setting"]);
            advanceScene->m_FrontScreenTable.back().grid->fromJson(screen["grid_setting"]);
        }
        //
        advanceScene->selectScreen(advanceScene->m_FrontScreenTable.front().name);
    }

    Grid::Ptr SceneManager::getScreenGrid()
    {
        return m_AdvancedScene->getFrontScreenGrid();
    }

    Grid::Ptr SceneManager::getWorldGrid()
    {
        return m_AdvancedScene->m_Grid;
    }

    void SceneManager::setUpdateUI(std::function<void()>  fn)
    {
        m_UpdateUI = fn;
    }

    void SceneManager::setUpdateUndo(std::function<void()>  fn)
    {
        m_UpdateUndo = fn;
    }

    void SceneManager::setUpdateLog(std::function<void(const std::string&, int)>  fn)
    {
        m_UpdateLog = fn;
    }

    void SceneManager::setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn)
    {
        m_UpdateLogIf = fn;
    }

    const std::vector<sf::String>& SceneManager::getSceneTable() const
    {
        return m_SceneTable;
    }

    std::vector<std::string> SceneManager::getScreenTable()
    {
        return m_AdvancedScene->getScreenTable();
    }

    bool SceneManager::addScreen(const std::string& name)
    {
       return m_AdvancedScene->addScreen(name);
    }

    bool SceneManager::deleteScreen(const std::string& name)
    {
        return m_AdvancedScene->deleteScreen(name);
    }

    bool SceneManager::renameScreen(const std::string& name, const std::string& newName)
    {
        return m_AdvancedScene->renameScreen(name, newName);
    }

    void SceneManager::selectScreen(const std::string& name)
    {
        m_AdvancedScene->selectScreen(name);
    }

    void SceneManager::updateSceneSaveFile()
    {
        std::string file = WORKSPACE_FOLDER + "/" +  m_AdvancedScene->m_SceneName + "/" + m_AdvancedScene->m_SceneName +  ".json";

        m_AdvancedScene->m_SceneBuilder->updateLayerOrder();

        saveFile(file, saveScene().dump(3));
    }

    void SceneManager::loadSceneSaveFile()
    {
        std::string file = WORKSPACE_FOLDER + "/" +  m_AdvancedScene->m_SceneName + "/" + m_AdvancedScene->m_SceneName +  ".json";

        m_UpdateLog("loading Scene [" + m_AdvancedScene->m_SceneName + "] from " + file, nero::Info);

        loadScene(loadJson(file));
    }

    void SceneManager::saveAllScene()
    {
        for(auto factory : m_SceneFactoryMap)
        {
            factory.second.first->m_SceneBuilder->updateLayerOrder();

            std::string file = WORKSPACE_FOLDER + "/" +  factory.first + "/" + factory.first +  ".json";

            saveFile(file, saveScene(factory.second.first).dump(3));
        }
    }

    Scene::Ptr SceneManager::getScene()
    {
        Scene::Ptr scene = m_SceneFactoryMap[m_AdvancedScene->m_SceneName].second();
        scene->m_UpdateLog = m_UpdateLog;
        scene->m_UpdateLogIf = m_UpdateLogIf;

        //World
        m_AdvancedScene->m_SceneBuilder->setPhysicWorld(scene->m_PhysicWorld);
        m_AdvancedScene->m_SceneBuilder->buildScene(scene->m_World);
        //Screen
        for(auto& devScreen : m_AdvancedScene->m_FrontScreenTable)
        {
            Screen::Ptr screen = Screen::Ptr(new Screen());

            //FrontScreen
            screen->screen  = Object::Ptr(new Object());
            screen->screenUI = UIObject::Ptr(new UIObject());

            devScreen.screenBuilder->buildScene(screen->screen);
            devScreen.screenBuilder->buildUI(screen->screenUI);

            screen->name = devScreen.name;
            screen->hide = true;
            screen->canvasColor = devScreen.screenBuilder->getCanvasColor();

            scene->m_ScreenTable.push_back(screen);
        }

        scene->m_ObjectManager->setObjectCount(m_AdvancedScene->m_SceneBuilder->getObjectCount());
        scene->m_CameraSetting  = m_AdvancedScene->m_CameraSetting;

        return scene;
    }

    sf::Vector2f SceneManager::getSceneResolution(Scene::Ptr scene)
    {
        return scene->getSceneResolution();
    }
}
