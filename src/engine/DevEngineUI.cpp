////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
#include <Nero/engine/DevEngineUI.h>
#include <Nero/utility/Utility.h>
#include <SFGUI/Separator.hpp>
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    DevEngineUI::DevEngineUI(sf::RenderWindow& window, ResourceManager::Ptr resourceManager):
        m_Sfgui()
        ,m_Window(window)
        ,m_ResourceManager(resourceManager)
        ,m_EngineMode(SCENE)
        ,m_EngineSubMode(OBJECT)
        ,m_SceneManager(nullptr)
        ,m_ActiveSceneBuilder(nullptr)
        ,m_ActiveMeshEditor(nullptr)
        ,m_ActiveUndoManager(nullptr)
        ,m_ActiveSoundManager(nullptr)
        ,m_SoundManager(nullptr)
        ,m_Grid(nullptr)
        ,m_Camera(nullptr)
        ,m_CanvasColor(sf::Color::Black)
        ,m_MousePositionText("")
        ,m_EngineSetting()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleEvent(sf::Event& event)
    {
        //UI
        m_Desktop.HandleEvent(event);

        //Scene Builder
        if(m_MouseOnCanvas && m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->handleEvent(event);

        //Mesh Editor
        if(m_MouseOnCanvas && m_EngineMode == SCENE && m_EngineSubMode == MESH)
            m_ActiveMeshEditor->handleEvent(event);

        //Scene Manager
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
            m_SceneManager->handleEvent(event);

        //Camera
        if(m_MouseOnCanvas)
            m_Camera->handleEvent(event);

         //Engine default Event handling
        switch(event.type)
        {
            case sf::Event::Closed:
                onQuitButton();
                break;

            //Keyboard
            case sf::Event::KeyPressed:
                handleKeyboardInput(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboardInput(event.key.code, false);
                break;

            //Mouse movements event
            case sf::Event::MouseMoved:
                handleMouseMoveInput(event.mouseMove);
                break;

            //Mouse wheel event
            case sf::Event::MouseWheelScrolled:
                handleMouseWheelInput(event.mouseWheelScroll);
                break;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::update(const sf::Time& timeStep)
    {
        //UI
        m_Desktop.Update(timeStep.asSeconds());

        //Camera
        m_Camera->update(timeStep);
        m_RenderCanvas->SetView(m_Camera->getView());

        //Grid
        m_Grid->update(m_Camera->getView());

        //Scene Builder
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->update(timeStep);

        //Scene Manager
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
            m_SceneManager->update(timeStep);

        //Update the current Scene camera Setting
        updateCameraSetting();

        //Auto save
        if(m_EngineSetting.autoSave)
            autoSave();

        //Position Label
        updatePositionLabel();


    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::render()
    {
        //Render Canvas
        m_RenderCanvas->Clear(m_CanvasColor);

            //Render the Grid on Front Screen
            if(m_SceneManager->getSceneSetting().drawGrid)
            {
                 m_RenderCanvas->SetView(m_CanvasFrontView);
                    m_RenderCanvas->Draw(*m_Grid);
                m_RenderCanvas->SetView(m_Camera->getView());
            }

            //Scene Builder
            if(m_EngineMode == SCENE && m_EngineSubMode != PLAY)
                m_ActiveSceneBuilder->render();

            //Scene Manager
            if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
                m_SceneManager->render();

            //Render on Front Screen
            m_RenderCanvas->SetView(m_CanvasFrontView);

                if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
                        m_SceneManager->renderOnFrontScreen();

                if(m_SceneManager->getSceneSetting().drawAxis)
                    m_Camera->render();

                m_RenderCanvas->Draw(m_Preview);

            m_RenderCanvas->SetView(m_Camera->getView());

        m_RenderCanvas->Display();

        //UI
        m_Sfgui.Display(m_Window);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build()
    {
        //UI properties
        m_Desktop.SetProperty("Label#little_font_size", "FontSize", 11.f);
        m_Desktop.SetProperty("Entry#sprite_layer", "BackgroundColor", sf::Color(249, 187,140, 100));
        m_Desktop.SetProperty("Entry#mesh_layer", "BackgroundColor", sf::Color(53, 80, 57));
        m_Desktop.SetProperty("Entry#meshed_layer", "BackgroundColor", sf::Color(53, 85, 101));
        m_Desktop.SetProperty("Entry#animation_layer", "BackgroundColor", sf::Color(229,162,8));
        m_Desktop.SetProperty("Entry#animation_meshed_layer", "BackgroundColor", sf::Color(128,168,202));
        m_Desktop.SetProperty("Entry#little_font_size" , "FontSize", 10.f);
        m_Desktop.SetProperty("SpinButton#little_font_size" , "FontSize", 10.f);
        m_Desktop.SetProperty("Notebook#layer_note_book", "BorderWidth", 0.f);
        m_Desktop.SetProperty("Notebook#layer_note_book", "Padding", 0.f);
        m_Desktop.SetProperty( "ScrolledWindow#help_window", "BorderColor", sf::Color::Transparent);

        //Build the UI
        auto main_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        build_left(main_box);
        build_center(main_box);
        build_right(main_box);
        m_Desktop.Add(main_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        if(isPressed)
        {
            if(key == sf::Keyboard::Return && !CTRL_SHIFT_ALT())
                onPlayButton();

            else if(key == sf::Keyboard::Return && !CTRL_SHIFT_ALT())
                onPauseButton();

            else if(key == sf::Keyboard::Return && CTRL())
                onResetButton();

            else if(key == sf::Keyboard::Left && CTRL())
                onPauseButton();

            else if(key == sf::Keyboard::Right && CTRL())
                onStepButton();

            else if(key == sf::Keyboard::Up && CTRL())
                onResetButton();

            else if(key == sf::Keyboard::Z && CTRL())
                onUndoButton();

            else if(key == sf::Keyboard::Y && CTRL())
                onRedoButton();

            else if(key == sf::Keyboard::S && CTRL())
                onSaveButton();

            else if(key == sf::Keyboard::Space && !CTRL_SHIFT_ALT())
                switchEngineSubMode();
        }
        else
        {
            if(key == sf::Keyboard::PageUp && !CTRL_SHIFT_ALT())
            {
                setEngineSubMode(OBJECT);

                onSaveButton();

                int index = m_SceneComboBox->GetSelectedItem() + 1;
                if(index > m_SceneManager->getSceneCount()-1)
                    index = 0;

                m_SceneComboBox->SelectItem(index);

                onSceneSelection();
            }

            else if(key == sf::Keyboard::PageDown && !CTRL_SHIFT_ALT())
            {
                setEngineSubMode(OBJECT);

                onSaveButton();

                int index = m_SceneComboBox->GetSelectedItem() - 1;
                if(index < 0)
                    index = m_SceneManager->getSceneCount()-1;

                m_SceneComboBox->SelectItem(index);
                onSceneSelection();
            }

            //Update Camera default state
            else if(key == sf::Keyboard::Numpad5 && ALT())
            {
                CameraSetting& setting = m_SceneManager->getCameraSetting();

                setting.defaultPosition = m_Camera->getPosition();
                setting.defaultRotation = m_Camera->getRotation();
                setting.defaultZoom     = m_Camera->getZoom();
            }

            //Rest Camera to default state
            if(key == sf::Keyboard::Numpad5 && !CTRL_SHIFT_ALT())
            {
                CameraSetting& setting = m_SceneManager->getCameraSetting();

                m_Camera->setPosition(setting.defaultPosition);
                m_Camera->setRotation(setting.defaultRotation);
                m_Camera->setZoom(setting.defaultZoom);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void  DevEngineUI::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        //update mouse position text
        if(m_MouseOnCanvas)
        {
            sf::Vector2f canvas_pos = sf::Vector2f(mouse.x - m_RenderCanvas->GetAbsolutePosition().x, mouse.y - m_RenderCanvas->GetAbsolutePosition().y);
            sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_RenderCanvas);

            m_MousePositionText =   "Canvas: x = " + toString(canvas_pos.x) + "   y = " + toString(canvas_pos.y) + \
                                    "  |----|  "
                            "World: x = " + toString(world_pos.x) + "   y = " + toString(world_pos.y) + \
                            "  |----|  ";
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse)
    {
        if (mouse.wheel == sf::Mouse::VerticalWheel && !CTRL_SHIFT_ALT())
        {
            if(mouse.delta > 0.f)
            {
                m_Preview.scale(sf::Vector2f(1.1f, 1.1f));
            }
            else
            {
                m_Preview.scale(sf::Vector2f(0.9f, 0.9f));
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateLog(const std::string& message)
    {
        m_LogLabel->SetText(m_LogLabel->GetText() + " " +  message + "\n");
        m_LogAdjustment->SetValue(m_LogAdjustment->GetUpper());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setSceneManager(SceneManager::Ptr sceneManager)
    {
       m_SceneManager = sceneManager;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setSoundManager(SoundManager::Ptr soundManager)
    {
       m_SoundManager = soundManager;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setCamera(AdvancedCamera::Ptr camera)
    {
        m_Camera = camera;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setEngineSetting(EngineSetting engineSetting)
    {
        m_EngineSetting = engineSetting;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setGrid(Grid::Ptr grid)
    {
        m_Grid = grid;
    }

    ////////////////////////////////////////////////////////////
    sfg::Canvas::Ptr DevEngineUI::getRenderCanvas() const
    {
        return m_RenderCanvas;
    }

    ////////////////////////////////////////////////////////////
    sf::View& DevEngineUI::getCanvasFrontView()
    {
        return m_CanvasFrontView;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::addScene(std::string name)
    {
        m_SceneComboBox->InsertItem(0, name);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::selectScene(std::string name)
    {
        m_SceneComboBox->SelectItem(m_SceneComboBox->GetItemCount()-1);

        for(int i=0; i < m_SceneComboBox->GetItemCount(); i++)
        {
            if(m_SceneComboBox->GetItem(i) == name)
            {
                m_SceneComboBox->SelectItem(i);
                break;
            }
        }

        onSceneSelection();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onSceneSelection()
    {
        //Select the Scene
        m_SceneManager->setScene(m_SceneComboBox->GetSelectedText());

        //Update the Active Managers
        m_ActiveSceneBuilder          = m_SceneManager->getSceneBuilder();
        m_ActiveMeshEditor            = m_SceneManager->getMeshEditor();
        m_ActiveUndoManager           = m_SceneManager->getUndoManager();
        m_ActiveSoundManager          = m_SceneManager->getSoundManager();

        //Update the Camera
        CameraSetting& cameraSetting = m_SceneManager->getCameraSetting();
        m_Camera->setPosition(cameraSetting.position);
        m_Camera->setRotation(cameraSetting.rotation);
        m_Camera->setZoom(cameraSetting.zoom);

        //Update the Layer Table
        updateLayerTable();

        //Update the Scene Setting
        updateSceneSetting();

        //Update Canvas Color
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onEngineModeNotebook()
    {
        //Currently there is only one Mode, this callback is therefore unnecessary.
        return;

        switch(m_EngineModeNotebook->GetCurrentPage())
        {
            case 0: //SCENE
            {
                m_EngineMode                    = SCENE;
                m_ActiveSceneBuilder            = m_SceneManager->getSceneBuilder();
                m_ActiveMeshEditor              = m_SceneManager->getMeshEditor();
                m_ActiveUndoManager             = m_SceneManager->getUndoManager();
                m_ActiveSoundManager            = m_SceneManager->getSoundManager();

                updateLog("current mode : Scene");

            }break;

            case 1: //Help
            {
               /*
                    Currently the HELP Mode does not exist.
                    The HELP Mode could be a interactive Scene that let the user learn more about the Engine;
               */

            }break;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onEngineSubModeButton()
    {
        if(m_ObjectModeRadioButton->IsActive())
        {
            m_EngineSubMode = OBJECT;
            m_ActiveSceneBuilder->updateAllLayerAlpha(DEFAULT_ALPHA);
            updateUndo();

            updateLog("current mode : Scene - Object");
        }
        else if(m_MeshModeRadioButton->IsActive())
        {
            m_EngineSubMode = MESH;
            m_ActiveSceneBuilder->updateAllLayerAlpha(MESH_MODE_ALPHA);
            updateUndo();

            updateLog("current mode : Scene - Mesh");
        }
        else if(m_PlayModeRadioButton->IsActive())
        {
            m_EngineSubMode = PLAY;
            m_ActiveSceneBuilder->updateAllLayerAlpha(DEFAULT_ALPHA);
            updateLog("current mode : Scene - Play");
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::switchEngineSubMode()
    {
        if(m_EngineSubMode == OBJECT && !CTRL_SHIFT_ALT())
        {
            setEngineSubMode(MESH);
        }
        else if(m_EngineSubMode != OBJECT && !CTRL_SHIFT_ALT())
        {
            setEngineSubMode(OBJECT);
        }
        else if(CTRL() && m_EngineSubMode != PLAY)
        {
            setEngineSubMode(PLAY);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setEngineSubMode(EngineSubMode mode)
    {
        switch(mode)
        {
            case OBJECT:
            {
                m_EngineSubMode = OBJECT;
                m_ActiveSceneBuilder->updateAllLayerAlpha(DEFAULT_ALPHA);
                m_ObjectModeRadioButton->SetActive(true);

                updateLog("current mode : Scene - Object");

            }break;

            case MESH:
            {
                m_EngineSubMode = MESH;
                m_ActiveSceneBuilder->updateAllLayerAlpha(MESH_MODE_ALPHA);
                m_MeshModeRadioButton->SetActive(true);

                updateLog("current mode : Scene - Mesh");

            }break;

            case PLAY:
            {
                m_EngineSubMode = PLAY;
                m_PlayModeRadioButton->SetActive(true);

                updateLog("current mode : Scene - Play");

            }break;
        }
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::onPlayButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            m_SceneManager->buildScene();
            setEngineSubMode(PLAY);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onPauseButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            m_SceneManager->getSceneSetting().pause = !m_SceneManager->getSceneSetting().pause;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onStepButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            m_SceneManager->getSceneSetting().pause         = true;
            m_SceneManager->getSceneSetting().singleStep    = true;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onResetButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            m_SceneManager->buildScene();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onQuitButton()
    {
        //Save Engine Setting
        nlohmann::json engine_setting = m_EngineSetting.toJson();

        //Save current Scene

        //Quit
        m_Window.close();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onUndoButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode == PLAY)
            return;

        nlohmann::json undo = m_ActiveUndoManager->undo();

        if(!undo.empty())
        {
            m_ActiveSceneBuilder->loadScene(undo["scene"]);
            updateLayerTable();

            if(m_ObjectModeRadioButton->IsActive())
                m_ActiveSceneBuilder->updateAllLayerAlpha(DEFAULT_ALPHA);
            else if(m_MeshModeRadioButton->IsActive())
                m_ActiveSceneBuilder->updateAllLayerAlpha(MESH_MODE_ALPHA);
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onRedoButton()
    {

        if(m_EngineMode != SCENE || m_EngineSubMode == PLAY)
            return;

        nlohmann::json redo = m_ActiveUndoManager->redo();

        if(!redo.empty())
        {
            m_ActiveSceneBuilder->loadScene(redo["scene"]);
            updateLayerTable();

            if(m_ObjectModeRadioButton->IsActive())
                m_ActiveSceneBuilder->updateAllLayerAlpha(DEFAULT_ALPHA);
            else if(m_MeshModeRadioButton->IsActive())
                m_ActiveSceneBuilder->updateAllLayerAlpha(MESH_MODE_ALPHA);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onSaveButton()
    {
        //update layers order
        m_ActiveSceneBuilder->updateLayerOrder();

        //
        std::string sceneName = m_SceneManager->getSceneName();
        std::string path = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

        nlohmann::json scene = m_ActiveSceneBuilder->saveScene();
        scene["name"] = sceneName;

        std::ofstream stream(path);

        if(!stream.is_open())
            return;

        stream << scene.dump(3);
        stream.close();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onLoadButton()
    {
        std::string sceneName = m_SceneComboBox->GetSelectedText();
        std::string path = WORKSPACE_FOLDER + "/" + sceneName + "/" + sceneName +  ".json";

        using namespace boost::filesystem;

        if(!exists(path))
            return;

        std::ifstream stream(path);

        if(!stream.is_open())
            return;

        nlohmann::json scene;

        stream >> scene;
        stream.close();

        m_ActiveSceneBuilder->loadScene(scene);

        updateLayerTable();

        updateUndo();
    }

























    void DevEngineUI::build_center(sfg::Box::Ptr main_box)
    {
        ////////////////////////////////////////////////////////////
        //Center : Mouse_Position| Utility_Button | Render_Canvas | Log_Window

        float win_width = m_Window.getSize().x;
        float win_height = m_Window.getSize().y;

        m_PositionLabel = sfg::Label::Create();
        m_PositionLabel->SetId("little_font_size");

        /*
          m_MousePositionLabel

        */

        //Utility_Button
        auto black_color_button     = sfg::Button::Create("Black");
        auto gray_color_button      = sfg::Button::Create("Gray");
        auto blue_color_button      = sfg::Button::Create("Blue");
        auto white_color_button     = sfg::Button::Create("White");

        m_PlayButton                = sfg::Button::Create("Play");
        m_PauseButton               = sfg::Button::Create("Pause");
        m_StepButton                = sfg::Button::Create("Step");
        m_ResetButton               = sfg::Button::Create("Reset");
        m_QuitButton                = sfg::Button::Create("Quit");
        m_UndoButton                = sfg::Button::Create("<--");
        m_RedoButton                = sfg::Button::Create("-->");

        black_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_CanvasColor = sf::Color::Black; onColorRadioButton();});
        gray_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_CanvasColor = sf::Color(119, 119, 119); onColorRadioButton();});
        blue_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_CanvasColor = sf::Color(130, 177, 205); onColorRadioButton();});
        white_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_CanvasColor = sf::Color::White; onColorRadioButton();});

        m_PlayButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onPlayButton, this));
        m_PauseButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onPauseButton, this));
        m_StepButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onStepButton, this));
        m_ResetButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onResetButton, this));
        m_QuitButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onQuitButton, this));
        m_UndoButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onUndoButton, this));
        m_RedoButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onRedoButton, this));

        auto utility_button_box     = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 80.f);
        auto utility_button_box_1   = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        auto utility_button_box_2   = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
        auto utility_button_box_3   = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);

        utility_button_box_1->Pack(black_color_button);
        utility_button_box_1->Pack(gray_color_button);
        utility_button_box_3->Pack(blue_color_button);
        utility_button_box_3->Pack(white_color_button);

        utility_button_box_2->Pack(m_UndoButton);
        utility_button_box_2->Pack(m_PlayButton);
        utility_button_box_2->Pack(m_PauseButton);
        utility_button_box_2->Pack(m_StepButton);
        utility_button_box_2->Pack(m_ResetButton);
        utility_button_box_2->Pack(m_QuitButton);
        utility_button_box_2->Pack(m_RedoButton);


        utility_button_box->Pack(utility_button_box_1);
        utility_button_box->Pack(utility_button_box_2);
        utility_button_box->Pack(utility_button_box_3);

        /*
            m_RenderCanvasColor

            m_PlayButton
            m_PauseButton
            m_StepButton
            m_ResetButton
            m_QuitButton

            onPlayButton()
            onPauseButton()
            onStepButton()
            onResetButton()
            onQuitButton()
        */

        //Render_Canvas
        m_RenderCanvas              = sfg::Canvas::Create();
        m_CanvasFrontView           = m_RenderCanvas->GetView();
        float frontViewWidth      = win_width*0.15f*3.63f-11.f;
        float frontViewHeight     =  win_height*0.75f-11.f;
        m_CanvasFrontView.setCenter(frontViewWidth/2.f, frontViewHeight/2.f);
        m_CanvasFrontView.setSize(sf::Vector2f(frontViewWidth, frontViewHeight));

        auto render_canvas_window   = sfg::Window::Create();
        render_canvas_window->SetStyle(sfg::Window::Style::BACKGROUND);
        render_canvas_window->SetRequisition(sf::Vector2f(win_width*0.15f*3.63f, win_height*0.79f));
        render_canvas_window->Add(m_RenderCanvas);

        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseEnter).Connect([this](){m_MouseOnCanvas = true;});
        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseLeave).Connect([this](){m_MouseOnCanvas = false;});

        //Log_Window
        m_LogLabel = sfg::Label::Create();
        auto log_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto log_window = sfg::ScrolledWindow::Create();
        log_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_NEVER);
        log_window->AddWithViewport(log_window_box);
        log_window->SetRequisition(sf::Vector2f(win_width*0.15f*3.63f, win_height*0.15f+4.f));
        log_window_box->Pack(m_LogLabel);
        m_LogAdjustment = log_window->GetVerticalAdjustment();

        /*
            m_LogLabel
            win_width
            win_height
        */

        //Center_End
        auto center_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        center_box->Pack(m_PositionLabel, false);
        center_box->Pack(utility_button_box, false);
        center_box->Pack(render_canvas_window);
        center_box->Pack(log_window);

        main_box->Pack(center_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_left(sfg::Box::Ptr main_box)
    {
        //Left : Top_Notebook | Bottom_Notebook

        float win_width = m_Window.getSize().x;
        float win_height = m_Window.getSize().y;

        ////////////////////////////////////////////////////////////
        //Top_Notebook : Utility | Music
        //Utility
        auto utility_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_utility_box(utility_view_box);
        //Music
        auto music_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_music_box(music_view_box);
        //Music
        auto grid_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_grid_box(grid_view_box);
        //Grid
        auto grid_main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_grid_box(grid_main_box);

        auto top_notebook = sfg::Notebook::Create();
        top_notebook->AppendPage(utility_view_box, sfg::Label::Create("Utility"));
        top_notebook->AppendPage(music_view_box, sfg::Label::Create("Music"));
        top_notebook->AppendPage(grid_view_box, sfg::Label::Create("Grid"));

        ////////////////////////////////////////////////////////////
        //Bottom_Notebook : Sprite | Animation | Color | Mesh
        //Sprite
        auto sprite_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        auto sprite_window = sfg::ScrolledWindow::Create();
            sprite_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
            sprite_window->AddWithViewport(sprite_box);
            sprite_window->SetRequisition(sf::Vector2f(win_width*0.14f, win_height*0.54f));
        build_sprite_box(sprite_box);
        sprite_box->GetSignal(sfg::Button::OnMouseLeave).Connect(std::bind(&DevEngineUI::onClosePreview, this));
        //Animation
        auto animation_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        auto animation_window = sfg::ScrolledWindow::Create();
            animation_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
            animation_window->AddWithViewport(animation_box);
            animation_window->SetRequisition(sf::Vector2f(win_width*0.14f, win_height*0.54f));
        build_animation_box(animation_box);
        animation_box->GetSignal(sfg::Button::OnMouseLeave).Connect(std::bind(&DevEngineUI::onClosePreview, this));
        //Color
        auto color_Window = sfg::Window::Create();
        color_Window->SetStyle(sfg::Window::Style::BACKGROUND);
        color_Window->SetRequisition(sf::Vector2f(win_width*0.14f, 0.f));
        build_color_window(color_Window);
        //Mesh
        auto mesh_Window = sfg::Window::Create();
        mesh_Window->SetStyle(sfg::Window::Style::BACKGROUND);
        mesh_Window->SetRequisition(sf::Vector2f(win_width*0.14f, 0.f));
        build_mesh_window(mesh_Window);
        //Sequence
        m_SequenceBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        m_SequenceNoteBook = sfg::Notebook::Create();
        m_SequenceNoteBook->SetId("layer_note_book");
        m_SequenceNoteBook->SetTabPosition(sfg::Notebook::TabPosition::BOTTOM);
        m_SequenceNoteBook->AppendPage(m_SequenceBox, sfg::Label::Create(""));

        auto sequence_window = sfg::ScrolledWindow::Create();
        sequence_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
        sequence_window->AddWithViewport(m_SequenceNoteBook);
        sequence_window->SetRequisition(sf::Vector2f(1100.f*0.14f, 576.f*0.56f));

        //
        auto bottom_notebook = sfg::Notebook::Create();
        bottom_notebook->AppendPage(sprite_window, sfg::Label::Create("Sprite"));
        bottom_notebook->AppendPage(animation_window, sfg::Label::Create("Animation"));
        bottom_notebook->AppendPage(color_Window, sfg::Label::Create("Color"));
        bottom_notebook->AppendPage(mesh_Window, sfg::Label::Create("Mesh"));
        bottom_notebook->AppendPage(sequence_window, sfg::Label::Create("Sequence"));

        bottom_notebook->SetScrollable(true);
        bottom_notebook->SetRequisition(sf::Vector2f(1100.f*0.12f, 0.f));

        //Left_End
        auto left_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        left_box->Pack(top_notebook);
        left_box->Pack(bottom_notebook);

        main_box->Pack(left_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_utility_box(sfg::Box::Ptr main_box)
    {
        //
        m_ObjectModeRadioButton = sfg::RadioButton::Create("Object");
        m_MeshModeRadioButton   = sfg::RadioButton::Create("Mesh",  m_ObjectModeRadioButton->GetGroup());
        m_PlayModeRadioButton   = sfg::RadioButton::Create("Play",  m_ObjectModeRadioButton->GetGroup());

        m_SaveButton            = sfg::Button::Create("Save");
        m_LoadButton            = sfg::Button::Create("Load");

        m_AutoSaveChekButton = sfg::CheckButton::Create("Auto save");

        //
        m_ObjectModeRadioButton->SetActive(true);
        m_AutoSaveChekButton->SetActive(m_EngineSetting.autoSave);

        //
        auto sub_mode_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        sub_mode_box->Pack(m_ObjectModeRadioButton, false);
        sub_mode_box->Pack(m_MeshModeRadioButton, false);
        sub_mode_box->Pack(m_PlayModeRadioButton, false);

        auto sub_mode_frame = sfg::Frame::Create("Mode");
        sub_mode_frame->Add(sub_mode_box);

        auto save_load_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
        save_load_box->Pack(m_SaveButton);
        save_load_box->Pack(m_LoadButton);

        auto save_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        save_box->Pack(m_AutoSaveChekButton, false);
        save_box->Pack(save_load_box, false);

        auto save_frame = sfg::Frame::Create("Save");
        save_frame->Add(save_box);

        //
        main_box->Pack(sub_mode_frame);
        main_box->Pack(save_frame);

        //
        m_ObjectModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_MeshModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_PlayModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_SaveButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onSaveButton, this));
        m_LoadButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onLoadButton, this));
        m_AutoSaveChekButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect([this](){m_EngineSetting.autoSave = m_AutoSaveChekButton->IsActive();});
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::build_music_box(sfg::Box::Ptr music_box)
    {
        //Play table
        m_MusicComboBox = sfg::ComboBox::Create();
        auto musicTable = m_ResourceManager->Music.getMusicTable();
        for(std::string music : musicTable)
        {
            m_MusicComboBox->AppendItem(music);
        }

        m_SoundComboBox = sfg::ComboBox::Create();
        auto soundTable = m_ResourceManager->Sound.getSoundBufferTable();
        for(std::string sound : soundTable)
        {
            m_SoundComboBox->AppendItem(sound);
        }

        //Volume
        m_MusicAdjustment    = sfg::Adjustment::Create(50.f, 0.f, 100.f);
        m_SoundAdjustment    = sfg::Adjustment::Create(50.f, 0.f, 100.f);

        m_MusicScaleBar = sfg::Scale::Create();
        m_MusicScaleBar->SetAdjustment(m_MusicAdjustment);
        m_MusicScaleBar->SetRequisition(sf::Vector2f(80.f, 15.f));
        m_SoundScaleBar = sfg::Scale::Create();
        m_SoundScaleBar->SetAdjustment(m_SoundAdjustment);
        m_SoundScaleBar->SetRequisition(sf::Vector2f(80.f, 15.f));

        auto volume_table = sfg::Table::Create();
        volume_table->Attach(sfg::Label::Create("Music"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        volume_table->Attach(m_MusicScaleBar, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        volume_table->Attach(sfg::Label::Create("Sound"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        volume_table->Attach(m_SoundScaleBar, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        //Listener
        auto listener_table = sfg::Table::Create();


        auto play_frame = sfg::Frame::Create("Play");
        auto play_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        play_box->Pack(m_MusicComboBox);
        play_box->Pack(m_SoundComboBox);
        play_frame->Add(play_box);

        auto volume_frame = sfg::Frame::Create("Volume");
        volume_frame->Add(volume_table);

        auto listener_frame = sfg::Frame::Create("Listener");
        listener_frame->Add(listener_table);

        auto stopMusicButton = sfg::Button::Create("Stop Music");

        music_box->Pack(play_frame);
        music_box->Pack(volume_frame);
        music_box->Pack(stopMusicButton);

        m_MusicComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onPlayMusic, this));
        m_SoundComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onPlaySound, this));
        stopMusicButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onStopMusic, this));
        m_MusicAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onMusicAdjustment, this));
        m_SoundAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onSoundAdjustment, this));

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_sprite_box(sfg::Box::Ptr sprite_box)
    {
        auto spriteTable = m_ResourceManager->Texture.getSpriteTable();
        int spriteCount = spriteTable.size();

        for(int i = 0; i < spriteCount; i++)
        {
            sfg::Button::Ptr button = sfg::Button::Create(spriteTable[i]);
            button->SetRequisition(sf::Vector2f(170.f, 0.f));

            button->GetSignal(sfg::Button::OnLeftClick).Connect(onSpriteButton(spriteTable[i]));
            button->GetSignal(sfg::Button::OnMouseEnter).Connect(onSpriteButtonMouseOver(spriteTable[i]));

            sprite_box->Pack(button);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_animation_box(sfg::Box::Ptr animation_box)
    {
        auto animationTable    = m_ResourceManager->Animation.getAnimationTable();
        auto animationCount    = animationTable.size();

        for(int i = 0; i < animationCount; i++)
        {
            sfg::Button::Ptr button = sfg::Button::Create(animationTable[i]);
            button->SetRequisition(sf::Vector2f(170.f, 0.f));

            button->GetSignal(sfg::Button::OnLeftClick).Connect(onAnimationButton(animationTable[i]));
            button->GetSignal(sfg::Button::OnMouseEnter).Connect(onAnimationButtonMouseOver(animationTable[i]));

            animation_box->Pack(button);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_color_window(sfg::Window::Ptr color_window)
    {
        m_SpriteColorRadioButton    = sfg::RadioButton::Create("Sprite");
        m_AnimationColorRadioButton    = sfg::RadioButton::Create("Animation");
        m_LayerColorRadioButton     = sfg::RadioButton::Create("Layer");
        m_CanvasColorRadioButton    = sfg::RadioButton::Create("Canvas");

        m_SpriteColorRadioButton->SetActive(true);
        m_LayerColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());
        m_CanvasColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());

        m_AlphaAdjustment   = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_RedAdjustment     = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_GreenAdjustment   = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_BlueAdjustment    = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);

        auto frame = sfg::Frame::Create("Choose");
        auto frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        frame_box->Pack(m_SpriteColorRadioButton);
        frame_box->Pack(m_LayerColorRadioButton);
        frame_box->Pack(m_CanvasColorRadioButton);
        frame->Add(frame_box);

        //
        auto alpha_scale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
        alpha_scale->SetAdjustment(m_AlphaAdjustment);
        alpha_scale->SetRequisition(sf::Vector2f(90.f, 0.f));
        auto alpha_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        alpha_box->Pack(sfg::Label::Create("Alpha"), true, false);
        alpha_box->Pack(alpha_scale);

        auto red_scale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
        red_scale->SetAdjustment(m_RedAdjustment);
        red_scale->SetRequisition(sf::Vector2f(90.f, 0.f));
        auto red_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        red_box->Pack(sfg::Label::Create("Red   "), true, false);
        red_box->Pack(red_scale);

        auto green_scale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
        green_scale->SetAdjustment(m_GreenAdjustment);
        green_scale->SetRequisition(sf::Vector2f(90.f, 0.f));
        auto green_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        green_box->Pack(sfg::Label::Create("Green"), true, false);
        green_box->Pack(green_scale);

        auto blue_scale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
        blue_scale->SetAdjustment(m_BlueAdjustment);
        blue_scale->SetRequisition(sf::Vector2f(90.f, 0.f));
        auto blue_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        blue_box->Pack(sfg::Label::Create("Blue  "), true, false);
        blue_box->Pack(blue_scale);

        alpha_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        red_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        green_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        blue_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});


        //
        auto alpha_spin = sfg::SpinButton::Create(m_AlphaAdjustment);
        alpha_spin->SetRequisition(sf::Vector2f(80.f, 0.f));
        alpha_spin->SetId("little_font_size");

        auto red_spin = sfg::SpinButton::Create(m_RedAdjustment);
        red_spin->SetRequisition(sf::Vector2f(80.f, 0.f));
        red_spin->SetId("little_font_size");

        auto green_spin = sfg::SpinButton::Create(m_GreenAdjustment);
        green_spin->SetRequisition(sf::Vector2f(80.f, 0.f));
        green_spin->SetId("little_font_size");

        auto blue_spin = sfg::SpinButton::Create(m_BlueAdjustment);
        blue_spin->SetRequisition(sf::Vector2f(80.f, 0.f));
        blue_spin->SetId("little_font_size");


        auto spin_table = sfg::Table::Create();
        spin_table->Attach(sfg::Label::Create("Alpha"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(alpha_spin, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Red"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(red_spin, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Green"), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(green_spin, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Blue "), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(blue_spin, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        alpha_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        red_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        green_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});
        blue_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){updateUndo();});

        //
        auto slider_frame = sfg::Frame::Create("Slider");
        auto slider_frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        slider_frame_box->Pack(alpha_box, false);
        slider_frame_box->Pack(red_box, false);
        slider_frame_box->Pack(green_box, false);
        slider_frame_box->Pack(blue_box, false);
        slider_frame->Add(slider_frame_box);

        auto spinner_frame = sfg::Frame::Create("Spinner");
        spinner_frame->Add(spin_table);

        auto color_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20.f);
            color_window_box->Pack(frame, false);
            color_window_box->Pack(slider_frame, false);
            color_window_box->Pack(spinner_frame, false);

        color_window->Add(color_window_box);

        m_AlphaAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_RedAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_GreenAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_BlueAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));

        m_SpriteColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_LayerColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_CanvasColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_mesh_window(sfg::Window::Ptr mesh_window)
    {
        m_MeshFixedRotationCheckButton      = sfg::CheckButton::Create("Fixed Rotation");
        m_MeshIsSensorCheckButton           = sfg::CheckButton::Create("Is Sensor");
        m_MeshAllowSleepCheckButton         = sfg::CheckButton::Create("Allow Sleep");

        m_MeshDensitySpinButton             = sfg::SpinButton::Create(0.f, 100.f, 1.f);
        m_MeshFrictionSpinButton            = sfg::SpinButton::Create(0.f, 10.f, 1.f);
        m_MeshRestitutionSpinButton         = sfg::SpinButton::Create(0.f, 10.f, 1.f);
        m_MeshGravityScaleSpinButton        = sfg::SpinButton::Create(0.f, 10.f, 1.f);


        m_MeshDensitySpinButton->SetRequisition(sf::Vector2f(70.f, 0.f));

        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Density         "), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshDensitySpinButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Friction         "), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshFrictionSpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Restitution     "), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshRestitutionSpinButton, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Gravity Scale "), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshGravityScaleSpinButton, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 20.f);
        window_box->Pack(m_MeshFixedRotationCheckButton, false);
        window_box->Pack(m_MeshIsSensorCheckButton, false);
        window_box->Pack(m_MeshAllowSleepCheckButton, false);
        window_box->Pack(table, false);

        mesh_window->Add(window_box);

        m_MeshFixedRotationCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onMeshFixedRotationButton, this));
        m_MeshIsSensorCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onMeshIsSensorButton, this));
        m_MeshAllowSleepCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onMeshAllowSleepButton, this));

        m_MeshDensitySpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onMeshDensityButton, this));
        m_MeshFrictionSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onMeshFrictionButton, this));
        m_MeshRestitutionSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onMeshRestitutionButton, this));
        m_MeshGravityScaleSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onMeshGravityScaleButton, this));
    }

    void DevEngineUI::build_right(sfg::Box::Ptr main_box)
    {
        //Mesh
        auto mesh_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_mesh_box(mesh_box);
        //Object
        auto object_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_object_box(object_box);
        //Layer
        auto layer_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        build_layer_box(layer_box);
        //left_box
        auto left_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        left_box->Pack(mesh_box, false);
        left_box->Pack(object_box, false);
        left_box->Pack(layer_box, false);
        //Help
        auto help_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto help_window = sfg::ScrolledWindow::Create();
            help_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_NEVER);
            help_window->AddWithViewport(help_box);
            help_window->SetId("help_window");
        auto helpLabel = sfg::Label::Create("Nero Game Engine v1.0");
            help_box->Pack(helpLabel);


        //Scene_Setting_Box
        auto right_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_scene_box(right_box);

        //Scene notebook box
        auto scene_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
            scene_box->Pack(left_box);
            scene_box->Pack(right_box);

        //Right_End
        m_EngineModeNotebook = sfg::Notebook::Create();
        m_EngineModeNotebook->AppendPage(scene_box, sfg::Label::Create("Scene"));
        m_EngineModeNotebook->AppendPage(help_window, sfg::Label::Create("Help"));
        m_EngineModeNotebook->GetSignal(sfg::Notebook::OnTabChange).Connect(std::bind(&DevEngineUI::onEngineModeNotebook, this));

        main_box->Pack(m_EngineModeNotebook);
    }

      ////////////////////////////////////////////////////////////
    void DevEngineUI::build_mesh_box(sfg::Box::Ptr main_box)
    {
        m_MeshComboBox = sfg::ComboBox::Create();
        m_MeshComboBox->AppendItem("Polygon");
        m_MeshComboBox->AppendItem("Circle");
        m_MeshComboBox->AppendItem("Line");

        m_MeshTypeComboBox = sfg::ComboBox::Create();
        m_MeshTypeComboBox->AppendItem("Static");
        m_MeshTypeComboBox->AppendItem("Kinematic");
        m_MeshTypeComboBox->AppendItem("Dynamic");

        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Add"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshComboBox, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Type "), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_MeshTypeComboBox, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );


        auto frame = sfg::Frame::Create("Mesh");
        frame->Add(table);

        main_box->Pack(frame, false);

        m_MeshComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onMeshComboBox, this));
        m_MeshTypeComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onMeshTypeComboBox, this));
    }

     void DevEngineUI::build_object_box(sfg::Box::Ptr main_box)
    {
        m_ObjectNameEntry           = sfg::Entry::Create();
        m_ObjectCategoryEntry       = sfg::Entry::Create();


        m_ObjectNameEntry->SetRequisition(sf::Vector2f(100.f, 0.f));
        m_ObjectCategoryEntry->SetRequisition(sf::Vector2f(100.f, 0.f));

        m_ObjectNameEntry->SetId("little_font_size");
        m_ObjectCategoryEntry->SetId("little_font_size");


        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Name    "), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_ObjectNameEntry, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Category"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_ObjectCategoryEntry, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto frame = sfg::Frame::Create("Object");
        frame->Add(table);

        main_box->Pack(frame, false);

        m_ObjectNameEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onObjectNameEntry, this));
        m_ObjectCategoryEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onObjectCategoryEntry, this));

        m_ObjectNameEntry->GetSignal(sfg::Entry::OnMouseLeave).Connect([this](){m_RenderCanvas->GrabFocus();});
        m_ObjectCategoryEntry->GetSignal(sfg::Entry::OnMouseLeave).Connect([this](){m_RenderCanvas->GrabFocus();});
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::build_layer_box(sfg::Box::Ptr main_box)
    {
        //Create Buttons
        m_AddLayerButton            = sfg::Button::Create("Add");
        m_DeleteLayerButton         = sfg::Button::Create("Del");
        m_UpLayerButton             = sfg::Button::Create("Up");
        m_DownLayerButton           = sfg::Button::Create("Down");

        m_MergeUpLayerButton        = sfg::Button::Create("Merge Up");
        m_MergeDownLayerButton      = sfg::Button::Create("Merge Down");

        //Create the layers box
        m_LayerBox              = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);

        //Create the layers scrolling window
        m_LayerNoteBook = sfg::Notebook::Create();
        m_LayerNoteBook->SetId("layer_note_book");
        m_LayerNoteBook->SetTabPosition(sfg::Notebook::TabPosition::BOTTOM);
        m_LayerNoteBook->AppendPage(m_LayerBox, sfg::Label::Create(""));

        auto window = sfg::ScrolledWindow::Create();
        window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
        window->AddWithViewport(m_LayerNoteBook);
        window->SetRequisition(sf::Vector2f(1100.f*0.17f, 576.f*0.56f));

        //Pack buttons
        auto button_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
            button_box->Pack(m_AddLayerButton);
            button_box->Pack(m_DeleteLayerButton);
            button_box->Pack(m_UpLayerButton);
            button_box->Pack(m_DownLayerButton);



        auto button_box_2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
            button_box_2->Pack(m_MergeUpLayerButton);
            button_box_2->Pack(m_MergeDownLayerButton);

        //Pack all
        auto layer_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
            layer_box->Pack(button_box, false);
            layer_box->Pack(sfg::Separator::Create());
            layer_box->Pack(button_box_2, false);
            layer_box->Pack(sfg::Separator::Create());
            layer_box->Pack(window);


        //Main Box
        auto frame = sfg::Frame::Create("Layer");
        frame->Add(layer_box);

        main_box->Pack(frame, false);

        //Callback
        m_AddLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAddLayerButton, this));
        m_DeleteLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onDeleteLayerButton, this));
        m_UpLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onUpLayerButton, this));
        m_DownLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onDownLayerButton, this));

        m_MergeUpLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onMergeUpLayerButton, this));
        m_MergeDownLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onMergeDownLayerButton, this));
    }

      ////////////////////////////////////////////////////////////
    void DevEngineUI::build_scene_box(sfg::Box::Ptr main_box)
    {
        m_SceneComboBox = sfg::ComboBox::Create();

        m_xGravitySpinButton = sfg::SpinButton::Create(-100.f, 100.f, 1.f);
        m_xGravitySpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_yGravitySpinButton = sfg::SpinButton::Create(-100.f, 100.f, 1.f);
        m_yGravitySpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));


        m_VelocityIterationSpinButton = sfg::SpinButton::Create(0.f, 500.f, 1.f);
        m_VelocityIterationSpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_PositionIterationSpinButton = sfg::SpinButton::Create(0.f, 100.f, 1.f);
        m_PositionIterationSpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_HertzSpinButton = sfg::SpinButton::Create(0.f, 200.f, 1.f);
        m_HertzSpinButton->SetRequisition( sf::Vector2f(49.f, 0.f ));

        auto spin_table = sfg::Table::Create();
        spin_table->Attach(sfg::Label::Create("Gravity X"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_xGravitySpinButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Gravity Y"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_yGravitySpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Hertz"), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_HertzSpinButton, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Velocity Itr"), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_VelocityIterationSpinButton, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Position Itr "), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_PositionIterationSpinButton, sf::Rect<sf::Uint32>( 1, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        m_SleepCheckButton                  = sfg::CheckButton::Create("Sleep");
        m_WarmStartingCheckButton           = sfg::CheckButton::Create("Warm Starting");
        m_TimeOfImpactCheckButton           = sfg::CheckButton::Create("Time of Impact");
        m_SubSteppingCheckButton            = sfg::CheckButton::Create("Sub-Setpping");

        m_DrawAxisCheckButton               = sfg::CheckButton::Create("Axis");
        m_DrawGridCheckButton               = sfg::CheckButton::Create("Grid");
        m_DrawShapesCheckButton             = sfg::CheckButton::Create("Shapes");
        m_DrawJointsCheckButton             = sfg::CheckButton::Create("Joints");
        m_DrawAABBsCheckButton              = sfg::CheckButton::Create("AABBs");
        m_DrawContactPointsCheckButton      = sfg::CheckButton::Create("Contact Points");
        m_DrawContactNormalsCheckButton     = sfg::CheckButton::Create("Contact Normals");
        m_DrawContactImpulsesCheckButton    = sfg::CheckButton::Create("Contact Impules");
        m_DrawFrictionImpulsesCheckButton   = sfg::CheckButton::Create("Friction impules");
        m_DrawCenterOfMassesCheckButton     = sfg::CheckButton::Create("Center of Masses");
        m_DrawStatisticsCheckButton         = sfg::CheckButton::Create("Statistics");
        m_DrawProfileCheckButton            = sfg::CheckButton::Create("Profile");

        auto scene_frame = sfg::Frame::Create("Scene");
        auto scene_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        scene_box->Pack(m_SceneComboBox, true, false);
        scene_box->Pack(spin_table, true, false);
        scene_box->Pack(m_SleepCheckButton, true, false);
        scene_box->Pack(m_WarmStartingCheckButton, true, false);
        scene_box->Pack(m_TimeOfImpactCheckButton, true, false);
        scene_box->Pack(m_SubSteppingCheckButton, true, false);
        scene_frame->Add(scene_box);

        auto draw_frame = sfg::Frame::Create("Draw");
        auto draw_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        draw_box->Pack(m_DrawAxisCheckButton);
        draw_box->Pack(m_DrawGridCheckButton);
        draw_box->Pack(m_DrawShapesCheckButton);
        draw_box->Pack(m_DrawJointsCheckButton);
        draw_box->Pack(m_DrawAABBsCheckButton);
        draw_box->Pack(m_DrawContactPointsCheckButton);
        draw_box->Pack(m_DrawContactNormalsCheckButton);
        draw_box->Pack(m_DrawContactImpulsesCheckButton);
        draw_box->Pack(m_DrawFrictionImpulsesCheckButton);
        draw_box->Pack(m_DrawCenterOfMassesCheckButton);
        draw_box->Pack(m_DrawStatisticsCheckButton);
        draw_box->Pack(m_DrawProfileCheckButton);
        draw_frame->Add(draw_box);

        main_box->Pack(scene_frame);
        main_box->Pack(draw_frame);


        ////////////////////////////////////////////////////////////

        m_SceneComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onSceneSelection, this));

        m_VelocityIterationSpinButton->GetSignal(sfg::SpinButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onScenePinButton, this));
        m_PositionIterationSpinButton->GetSignal(sfg::SpinButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onScenePinButton, this));
        m_HertzSpinButton->GetSignal(sfg::SpinButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onScenePinButton, this));

        m_SleepCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_WarmStartingCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_TimeOfImpactCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_SubSteppingCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));

        m_DrawAxisCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawGridCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawShapesCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawJointsCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawAABBsCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawContactPointsCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawContactNormalsCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawContactImpulsesCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawFrictionImpulsesCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawCenterOfMassesCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawStatisticsCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
        m_DrawProfileCheckButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onSceneCheckButton, this));
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::build_grid_box(sfg::Box::Ptr main_box)
    {
        m_xGridSpinButton         =   sfg::SpinButton::Create(5.f, 100.f, 1.f);
        m_yGridSpinButton         =   sfg::SpinButton::Create(5.f, 100.f, 1.f);

        m_xGridSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_yGridSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));

        m_xGridSpinButton->SetId("layer");
        m_yGridSpinButton->SetId("layer");


        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Width"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_xGridSpinButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Height"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_yGridSpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto frame = sfg::Frame::Create("Grid");
        frame->Add(table);

        main_box->Pack(frame, false);
    }






     ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onSpriteButton(const std::string &label)
    {
        return [&, label]()
        {
            if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

            sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
            sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

            if(CTRL())
            {
                if(m_ActiveSceneBuilder->addObject(Object::Meshed_Object, label, world_pos))
                    updateLayerTable();
            }
            else
            {
                if(m_ActiveSceneBuilder->addObject(Object::Sprite_Object, label, world_pos))
                    updateLayerTable();
            }
        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onSpriteButtonMouseOver(const std::string &label)
    {
        return [&, label]()
            {
                m_PreviewTexture = m_ResourceManager->Texture.getSpriteTexture(label);
                sf::IntRect bound = m_ResourceManager->Texture.getSpriteBound(label);
                m_Preview.setTextureRect(bound);
                m_Preview.setTexture(m_PreviewTexture);
                m_Preview.setOrigin(bound.width/2.f, bound.height/2.f);
                m_Preview.setPosition(m_CanvasFrontView.getSize().x/2.f, bound.height/2.f + 100.f);
                m_Preview.setColor(sf::Color(255, 255, 255, 100));
            };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onAnimationButton(const std::string &label)
    {
        return [&, label]()
        {
            if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

            sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
            sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

            if(CTRL())
            {
                if(m_ActiveSceneBuilder->addObject(Object::Animation_Meshed_Object, label, world_pos))
                    updateLayerTable();
            }
            else
            {
                if(m_ActiveSceneBuilder->addObject(Object::Animation_Object, label, world_pos))
                    updateLayerTable();
            }
        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onAnimationButtonMouseOver(const std::string &label)
    {
        return [&, label]()
            {
                m_PreviewTexture = m_ResourceManager->Animation.getTexture(label);
                sf::IntRect bound = m_ResourceManager->Animation.getAnimationBound(label);
                m_Preview.setTexture(m_PreviewTexture);
                m_Preview.setTextureRect(bound);
                m_Preview.setOrigin(bound.width/2.f, bound.height/2.f);
                m_Preview.setPosition(m_CanvasFrontView.getSize().x/2.f, bound.height/2.f + 100.f);
                m_Preview.setColor(sf::Color(255, 255, 255, 100));
            };
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onClosePreview()
    {
        m_Preview = sf::Sprite();
    }



    void DevEngineUI::updateUI()
    {
        Object::Ptr object = m_ActiveSceneBuilder->getSelectedObject();

        m_ObjectNameEntry->SetText(object->getName());
        m_ObjectCategoryEntry->SetText(object->getCategory());

        if((object->getFirstType() == Object::Sprite_Object || object->getFirstType() == Object::Animation_Object) && m_SpriteColorRadioButton->IsActive())
        {
            sf::Color c = object->getColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }

        if (object->getSecondType() == Object::Mesh_Object || object->getSecondType() == Object::Meshed_Object || object->getSecondType() == Object::Animation_Meshed_Object)
        {
            MeshObject::Ptr mesh_object;

            if(object->getSecondType() == Object::Mesh_Object)
                mesh_object = MeshObject::Cast(object);
            else
                mesh_object = MeshObject::Cast(object->getFirstChild());

            m_MeshFixedRotationCheckButton->SetActive(mesh_object->getMeshFixedRotation());
            m_MeshIsSensorCheckButton->SetActive(mesh_object->getMeshSensor());
            m_MeshAllowSleepCheckButton->SetActive(mesh_object->getMeshAllowSleep());

            m_MeshDensitySpinButton->SetValue(mesh_object->getMeshDensity());
            m_MeshFrictionSpinButton->SetValue(mesh_object->getMeshFriction()*10);
            m_MeshRestitutionSpinButton->SetValue(mesh_object->getMeshRestitution()*10);
            m_MeshGravityScaleSpinButton->SetValue(mesh_object->getMeshGravityScale());
        }

        if(object->getFirstType() == Object::Animation_Object)
        {
            m_SequenceBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
            m_SequenceNoteBook->AppendPage(m_SequenceBox, sfg::Label::Create(""));

            auto animation_object = AnimationObject::Cast(object);
            auto sequenceMap = animation_object->getSequenceMap();

            for(auto it = sequenceMap.rbegin(); it != sequenceMap.rend(); it++)
            {
                std::string sequence = it->first;
                auto frame = sfg::Frame::Create(sequence);
                auto box   = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);

                auto adjustment     = sfg::Adjustment::Create(1.f/it->second.getFrameRate(), 1.f, 100.f);
                auto spinButton     = sfg::SpinButton::Create(adjustment);
                spinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
                spinButton->SetId("little_font_size");
                auto spin_box   = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
                spin_box->Pack(sfg::Label::Create("Frame rate"));
                spin_box->Pack(spinButton);

                auto checkButton = sfg::CheckButton::Create("Loop");
                checkButton->SetActive(it->second.getLoop());

                auto selectButton = sfg::Button::Create("Select");

                selectButton->GetSignal(sfg::Button::OnLeftClick).Connect([this, animation_object, sequence]()
                  {
                    animation_object->setSequence(sequence);
                  });

                spinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect([this, animation_object, spinButton]()
                  {
                    animation_object->setFrameRate(1.f/spinButton->GetValue());
                  });

                checkButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect([this, animation_object, checkButton]()
                  {
                    animation_object->setLoop(checkButton->IsActive());
                  });


                box->Pack(checkButton);
                box->Pack(spin_box);
                box->Pack(selectButton);

                frame->Add(box);

                m_SequenceBox->Pack(frame);

            }

            m_SequenceNoteBook->RemovePage(0);
        }

    }

    void DevEngineUI::updateUndo()
    {
        nlohmann::json scene = m_ActiveSceneBuilder->saveScene();
        nlohmann::json undo;
        undo["scene"] = scene;
        undo["engine_sub_mode"] = m_EngineSubMode;
        undo["update_layer_tab"] = true;
        m_ActiveUndoManager->add(undo);
    }

    void DevEngineUI::updateLayerUndo()
    {
        nlohmann::json undo;
        undo["scene"] = m_ActiveSceneBuilder->saveScene();
        undo["engine_sub_mode"] = m_EngineSubMode;
        undo["update_layer_tab"] = true;

        m_ActiveUndoManager->add(undo);
    }

    ////////////////////////////////////////////////////////////
    //Layer
    DevEngineUI::LayerUI::LayerUI()
    {
        showToggleButton    = sfg::ToggleButton::Create("show");
        selectRadioButton   = sfg::RadioButton::Create("");
        nameEntry           = sfg::Entry::Create();

        nameEntry->SetRequisition(sf::Vector2f(100.f, 0.f));
    }

    ////////////////////////////////////////////////////////////
    sfg::Box::Ptr DevEngineUI::LayerUI::getLayerBox()
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
        box->Pack(showToggleButton, false);
        box->Pack(selectRadioButton, false);
        box->Pack(nameEntry);

        return box;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateLayerTable()
    {
        auto layerTable = m_ActiveSceneBuilder->getLayerTable();
        m_LayerTable.clear();

        for(auto layer : layerTable)
            createLayer(layer);

        updateLayerBox();
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::createLayer(LayerObject::Ptr layerObject)
    {
        LayerUI layer;
        layer.layerId = layerObject->getId();

        if(!m_LayerTable.empty())
            layer.selectRadioButton->SetGroup(m_LayerTable[0].selectRadioButton->GetGroup());

        layer.selectRadioButton->SetActive(layerObject->isSelected());
        layer.showToggleButton->SetActive(layerObject->isVisible());
        layer.nameEntry->SetText(layerObject->getName());

        switch(layerObject->getSecondType())
        {
            case Object::Sprite_Object: layer.nameEntry->SetId("sprite_layer"); break;
            case Object::Mesh_Object: layer.nameEntry->SetId("mesh_layer"); break;
            case Object::Meshed_Object: layer.nameEntry->SetId("meshed_layer"); break;
            case Object::Animation_Object: layer.nameEntry->SetId("animation_layer"); break;
            case Object::Animation_Meshed_Object: layer.nameEntry->SetId("animation_meshed_layer"); break;
        }

        m_LayerTable.push_back(layer);
        m_LayerTable.back().showToggleButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(onShowLayerButton(layerObject->getId()));
        m_LayerTable.back().selectRadioButton->GetSignal(sfg::RadioButton::OnMouseLeftPress).Connect(onSelectLayerButton(layerObject->getId()));
        m_LayerTable.back().nameEntry->GetSignal(sfg::Entry::OnKeyRelease).Connect(onLayerNameEntry(layerObject->getId()));
    }


    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateLayerBox()
    {
        m_LayerBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        m_LayerNoteBook->AppendPage(m_LayerBox, sfg::Label::Create(""));


        for(auto it = m_LayerTable.rbegin(); it != m_LayerTable.rend(); it++)
            m_LayerBox->Pack(it->getLayerBox(), false);

        m_LayerNoteBook->RemovePage(0);

    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onShowLayerButton(const int& layerId)
    {
        return [this, layerId]()
        {
             m_ActiveSceneBuilder->updateLayerVisibility(layerId);

        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onSelectLayerButton(const int& layerId)
    {
        return [this, layerId]()
        {
            m_ActiveSceneBuilder->selectLayer(layerId);
        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngineUI::onLayerNameEntry(const int& layerId)
    {
        return [this, layerId]()
        {
            for(auto layer : m_LayerTable)
                if(layer.layerId == layerId)
                    m_ActiveSceneBuilder->updateLayerName(layerId, layer.nameEntry->GetText());
        };
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::onAddLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        if(m_LayerTable.size() < SPRITE_LAYER_MAX)
        {
            auto layer = m_ActiveSceneBuilder->addLayer();
            createLayer(layer);
            updateLayerBox();

        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onDeleteLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        if(m_LayerTable.size() == 1)
        {
            m_ActiveSceneBuilder->deleteLayer(m_LayerTable.back().layerId);
            m_LayerTable.pop_back();
            onAddLayerButton();
        }
        else
        {
            for(auto it =  m_LayerTable.begin(); it !=  m_LayerTable.end(); it++)
            {
                if(it->selectRadioButton->IsActive())
                {
                    if(it != m_LayerTable.begin())
                        (it-1)->selectRadioButton->SetActive(true);
                    else if(it == m_LayerTable.begin() && m_LayerTable.size() != 1)
                        (it+1)->selectRadioButton->SetActive(true);

                    m_ActiveSceneBuilder->deleteLayer(it->layerId);

                    m_LayerTable.erase(it);

                    break;
                }
            }
        }

        updateLayerBox();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onUpLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != (m_LayerTable.end()-1))
            {
                 m_ActiveSceneBuilder->moveLayerUp(it->layerId);

                std::iter_swap(it, it++);
            }
        }

        updateLayerBox();
    }


    ////////////////////////////////////////////////////////////
    void DevEngineUI::onDownLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != m_LayerTable.begin())
            {
                 m_ActiveSceneBuilder->moveLayerDown(it->layerId);

                std::iter_swap(it, it--);
            }
        }

        updateLayerBox();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMergeUpLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != (m_LayerTable.end()-1))
            {
                if(m_ActiveSceneBuilder->mergeLayerUp(it->layerId))
                {
                    (it+1)->selectRadioButton->SetActive(true);

                    m_LayerTable.erase(it);
                     break;
                }
            }
        }

        updateLayerBox();
    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::onMergeDownLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTable.begin(); it != m_LayerTable.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != m_LayerTable.begin())
            {
                if(m_ActiveSceneBuilder->mergeLayerDown(it->layerId))
                {
                    (it-1)->selectRadioButton->SetActive(true);

                    m_LayerTable.erase(it);
                    break;
                }
            }
        }

        updateLayerBox();
    }

    ///////////////////////////////////////////////////////////
    void DevEngineUI::onObjectNameEntry()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->setObjectName(m_ObjectNameEntry->GetText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onObjectCategoryEntry()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->setObjectCategory(m_ObjectCategoryEntry->GetText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshComboBox()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
            sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

            if(m_ActiveSceneBuilder->addObject(Object::Mesh_Object, m_MeshComboBox->GetSelectedText(), world_pos))
                updateLayerTable();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshTypeComboBox()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->setMeshType(m_MeshTypeComboBox->GetSelectedText());
    }

    void DevEngineUI::onPlayMusic()
    {
        m_SoundManager->playMusic(toString(m_MusicComboBox->GetSelectedText()));
    }

    void DevEngineUI::onPlaySound()
    {
         m_SoundManager->playSound(toString(m_SoundComboBox->GetSelectedText()));
    }

    void DevEngineUI::onStopMusic()
    {
        m_SoundManager->stopMusic(toString(m_MusicComboBox->GetSelectedText()));
    }

    void DevEngineUI::onMusicAdjustment()
    {
        m_SoundManager->setMusicVolume(m_MusicAdjustment->GetValue());
    }

    void DevEngineUI::onSoundAdjustment()
    {
        m_SoundManager->setSoundVolume(m_SoundAdjustment->GetValue());
    }







    void DevEngineUI::updatePositionLabel()
    {
        std::string cameraPositionText = "Camera: x = " + toString(m_Camera->getPosition().x) + "   y = " + toString(m_Camera->getPosition().y);

        m_PositionLabel->SetText(m_MousePositionText + cameraPositionText);
    }





     ////////////////////////////////////////////////////////////
    void DevEngineUI::onColorRadioButton()
    {
        if(m_SpriteColorRadioButton->IsActive())
        {
            Object::Ptr object = m_ActiveSceneBuilder->getSelectedObject();

            if(object)
            {
                sf::Color c = object->getColor();

                m_AlphaAdjustment->SetValue(c.a);
                m_RedAdjustment->SetValue(c.r);
                m_GreenAdjustment->SetValue(c.g);
                m_BlueAdjustment->SetValue(c.b);
            }

        }
        else if(m_LayerColorRadioButton->IsActive())
        {
            /*sf::Color c = m_ActiveSceneBuilder->getLayerColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);*/

        }
        else if (m_CanvasColorRadioButton->IsActive())
        {
            sf::Color c = m_CanvasColor;

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onColorAdjustment()
    {
        sf::Color color(m_RedAdjustment->GetValue(), m_GreenAdjustment->GetValue(), m_BlueAdjustment->GetValue(), m_AlphaAdjustment->GetValue());

        if(m_LayerColorRadioButton->IsActive())
            m_ActiveSceneBuilder->updateLayerColor(color);
        else if(m_SpriteColorRadioButton->IsActive())
            m_ActiveSceneBuilder->updateSpriteColor(color);
        else if (m_CanvasColorRadioButton->IsActive())
            m_CanvasColor = color;
    }







    void DevEngineUI::updateSceneSetting()
    {
        SceneSetting& setting = m_SceneManager->getSceneSetting();

        m_xGravitySpinButton->SetValue(setting.gravity.x);
        m_yGravitySpinButton->SetValue(setting.gravity.y);

        m_VelocityIterationSpinButton->SetValue(setting.velocityIterations);
        m_PositionIterationSpinButton->SetValue(setting.positionIterations);
        m_HertzSpinButton->SetValue(setting.hz);

        m_SleepCheckButton->SetActive(setting.enableSleep);
        m_WarmStartingCheckButton->SetActive(setting.enableWarmStarting);
        m_TimeOfImpactCheckButton->SetActive(setting.enableContinuous);
        m_SubSteppingCheckButton->SetActive(setting.enableSubStepping);

        m_DrawAxisCheckButton->SetActive(setting.drawAxis);
        m_DrawGridCheckButton->SetActive(setting.drawGrid);
        m_DrawShapesCheckButton->SetActive(setting.drawShapes);
        m_DrawJointsCheckButton->SetActive(setting.drawJoints);
        m_DrawAABBsCheckButton->SetActive(setting.drawAABBs);
        m_DrawContactPointsCheckButton->SetActive(setting.drawContactPoints);
        m_DrawContactNormalsCheckButton->SetActive(setting.drawContactNormals);
        m_DrawContactImpulsesCheckButton->SetActive(setting.drawContactImpulse);
        m_DrawFrictionImpulsesCheckButton->SetActive(setting.drawFrictionImpulse);
        m_DrawCenterOfMassesCheckButton->SetActive(setting.drawCOMs);
        m_DrawStatisticsCheckButton->SetActive(setting.drawStats);
        m_DrawProfileCheckButton->SetActive(setting.drawProfile);

    }

     ////////////////////////////////////////////////////////////
    void DevEngineUI::onScenePinButton()
    {
        SceneSetting& setting = m_SceneManager->getSceneSetting();

        setting.gravity.x           =  m_xGravitySpinButton->GetValue();
        setting.gravity.y           =  m_yGravitySpinButton->GetValue();
        setting.velocityIterations  =  m_VelocityIterationSpinButton->GetValue();
        setting.positionIterations  =  m_PositionIterationSpinButton->GetValue();
        setting.hz                  =  m_HertzSpinButton->GetValue();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onSceneCheckButton()
    {
        SceneSetting& setting = m_SceneManager->getSceneSetting();

        setting.enableSleep           = m_SleepCheckButton->IsActive();
        setting.enableWarmStarting    = m_WarmStartingCheckButton->IsActive();
        setting.enableContinuous      = m_TimeOfImpactCheckButton->IsActive();
        setting.enableSubStepping     = m_SubSteppingCheckButton->IsActive();

        setting.drawAxis              = m_DrawAxisCheckButton->IsActive();
        setting.drawGrid              = m_DrawGridCheckButton->IsActive();
        setting.drawShapes            = m_DrawShapesCheckButton->IsActive();
        setting.drawJoints            = m_DrawJointsCheckButton->IsActive();
        setting.drawAABBs             = m_DrawAABBsCheckButton->IsActive();
        setting.drawContactPoints     = m_DrawContactPointsCheckButton->IsActive();
        setting.drawContactNormals    = m_DrawContactNormalsCheckButton->IsActive();
        setting.drawContactImpulse    = m_DrawContactImpulsesCheckButton->IsActive();
        setting.drawFrictionImpulse   = m_DrawFrictionImpulsesCheckButton->IsActive();
        setting.drawCOMs              = m_DrawCenterOfMassesCheckButton->IsActive();
        setting.drawStats             = m_DrawStatisticsCheckButton->IsActive();
        setting.drawProfile           = m_DrawProfileCheckButton->IsActive();
    }

    void DevEngineUI::updateCameraSetting()
    {
        CameraSetting& setting = m_SceneManager->getCameraSetting();

        setting.position    = m_Camera->getPosition();
        setting.rotation    = m_Camera->getRotation();
        setting.zoom        = m_Camera->getZoom();
    }

    void DevEngineUI::autoSave()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT && m_EngineSetting.autoSave)
        {
            m_ElapseTime += m_AutoSaveClock.restart();
            if(m_ElapseTime > sf::seconds(m_EngineSetting.autoSaveTime))
            {
                m_ElapseTime -= sf::seconds(m_EngineSetting.autoSaveTime);
                onSaveButton();
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshFixedRotationButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshFixedRotation(m_MeshFixedRotationCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshIsSensorButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshSensor(m_MeshIsSensorCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshAllowSleepButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshAllowSleep(m_MeshAllowSleepCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshDensityButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshDensity(m_MeshDensitySpinButton->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshFrictionButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshFriction(m_MeshFrictionSpinButton->GetValue()/10.f);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshRestitutionButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshRestitution(m_MeshRestitutionSpinButton->GetValue()/10.f);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshGravityScaleButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
           m_ActiveSceneBuilder->setMeshGravityScale(m_MeshGravityScaleSpinButton->GetValue());
    }






}
