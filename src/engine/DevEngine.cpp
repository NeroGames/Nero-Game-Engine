////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/engine/DevEngine.h>
//SFGUI
#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Table.hpp>
//EASY_LOG
#include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////

#include <SFML/Graphics/View.hpp>

namespace nero
{
    ////////////////////////////////////////////////////////////
    DevEngine::DevEngine(const sf::String& title, const float& winWidth, const float& winHeight):
        Engine(title, winWidth, winHeight)
        //Main
        ,m_Sfgui()
        ,m_Camera(sf::Vector2f(winWidth*0.15f*3.65f, winHeight*0.79f))
        ,m_ResourceManager()
        ,m_Current()
        ,m_SceneManager(Scene::Context(m_RenderCanvas, m_CanvasDefaultView, m_Camera, m_ResourceManager))
        //Engine Mode
        ,m_EngineMode(SCENE)
        ,m_EngineSubMode(OBJECT)
        //Canvas
        ,m_RenderCanvasColor(sf::Color::Black)
        ,m_MouseIsOnRenderCanvas(false)
        //Auto_Save
        ,m_AutoSave(true)
        ,m_AutoSaveClock()
        ,m_ElapseTime(sf::Time::Zero)
        //Sprite
        ,m_SpritePreview()
        ,m_TexturePreview()
    {
        //Load resources : Texture, Font, Music, Sound, Shader
        m_ResourceManager.Texture.load("Resource/Texture");
        m_ResourceManager.Font.load("Resource/Font");

        //Setup SFML window
        m_Window.setVerticalSyncEnabled(true);
        m_Window.resetGLStates();

        //Setup SFGUI desktop
        m_Desktop.SetProperty("Label#little_font_size", "FontSize", 11.f);
        m_Desktop.SetProperty("Entry#sprite_layer", "BackgroundColor", sf::Color(249, 187,140, 100));
        m_Desktop.SetProperty("Entry#mesh_layer", "BackgroundColor", sf::Color(53, 80, 57));
        m_Desktop.SetProperty("Entry#meshed_layer", "BackgroundColor", sf::Color(53, 85, 101));
        m_Desktop.SetProperty("Entry#little_font_size" , "FontSize", 10.f);
        m_Desktop.SetProperty("SpinButton#little_font_size" , "FontSize", 10.f);

        //Build the engine interface
        build_engine();

        //Add the default scene
        addScene<Scene>("Default");
        m_SceneComboBox->SelectItem(0);
        onSceneComboBox();
    }

    ////////////////////////////////////////////////////////////
    DevEngine::~DevEngine()
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::handleEvents()
    {
        sf::Event event;
        while(m_Window.pollEvent(event))
        {
            //SFGUI
            m_Desktop.HandleEvent(event);

            //Engine Components Event
            if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT && m_MouseIsOnRenderCanvas)
                m_Current.objectManager->handleEvent(event);

            else if (m_EngineMode == SCENE && m_EngineSubMode == MESH && m_MouseIsOnRenderCanvas)
                m_Current.meshEditor->handleEvent(event);

            else if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
                m_SceneManager.handleEvent(event);

            //Camera
            if(m_MouseIsOnRenderCanvas)
                m_Camera.handleEvent(event);

            //Engine default Event handling
            switch(event.type)
            {
                case sf::Event::Closed:
                    m_Window.close();
                    break;

                //Keyboard
                case sf::Event::KeyPressed:
                    handleKeyboardInput(event.key.code, true);
                    break;
                case sf::Event::KeyReleased:
                    handleKeyboardInput(event.key.code, false);
                    break;

                //Mouse mouvements event
                case sf::Event::MouseMoved:
                    handleMouseMoveInput(event.mouseMove);
                    break;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::update(const sf::Time& elapsedTime)
    {
        //SFGUI
        m_Desktop.Update(elapsedTime.asSeconds());

        //Camera
        m_Camera.update(elapsedTime);
        m_RenderCanvas->SetView(m_Camera.getView());

        //Scene camera settings
        CameraSettings& settings = m_SceneManager.getCameraSetting();
            settings.position_x     = m_Camera.getPosition().x;
            settings.position_y     = m_Camera.getPosition().y;
            settings.rotation       = m_Camera.getRotation();
            settings.zoom           = m_Camera.getZoom();


        //Engine Components Event
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
            m_SceneManager.update();

        else if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->update(elapsedTime);

        //Auto save
        m_ElapseTime += m_AutoSaveClock.restart();

        if(m_ElapseTime > AUTO_SAVE_TIME && m_AutoSave && m_EngineSubMode == OBJECT)
        {
            m_ElapseTime -= AUTO_SAVE_TIME;
            onSaveButton();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::render()
    {
          m_Window.clear(sf::Color(100, 100, 100));
            m_RenderCanvas->Clear(m_RenderCanvasColor);

                if(m_EngineMode == SCENE && m_EngineSubMode != PLAY)
                    m_Current.objectManager->render();

                if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
                {
                    m_SceneManager.render();
                    m_SceneManager.renderShape();
                    m_SceneManager.renderDebug();
                }


                m_RenderCanvas->SetView(m_CanvasDefaultView);
                    m_RenderCanvas->Draw(m_SpritePreview);
                    if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
                        m_SceneManager.renderOnFrontScreen();
                m_RenderCanvas->SetView(m_Camera.getView());


                m_RenderCanvas->Display();
            m_Sfgui.Display(m_Window);
        m_Window.display();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {

        if(isPressed)
        {
            if(key == sf::Keyboard::Return && !CTRL_SHIFT_ALT() && m_EngineSubMode == OBJECT)
                onPlayButton();

            else if(key == sf::Keyboard::P && !CTRL_SHIFT_ALT() && m_EngineSubMode == PLAY)
                onPauseButton();

            else if(key == sf::Keyboard::R && !CTRL_SHIFT_ALT() && m_EngineSubMode == PLAY)
                onResetButton();

            else if(key == sf::Keyboard::Left && !CTRL_SHIFT_ALT() && m_EngineSubMode == PLAY)
                onPauseButton();

            else if(key == sf::Keyboard::Right && !CTRL_SHIFT_ALT() && m_EngineSubMode == PLAY)
                onStepButton();

            else if(key == sf::Keyboard::Up && !CTRL_SHIFT_ALT() && m_EngineSubMode == PLAY)
                onResetButton();

            else if(key == sf::Keyboard::Z && CTRL() && m_EngineSubMode != PLAY)
                onUndoButton();

            else if(key == sf::Keyboard::Y && CTRL() && m_EngineSubMode != PLAY)
                onRedoButton();

            else if(key == sf::Keyboard::S && CTRL() && m_EngineSubMode != PLAY)
                onSaveButton();

            if(key == sf::Keyboard::Multiply && !CTRL_SHIFT_ALT())
                m_SpritePreview.scale(sf::Vector2f(1.1f, 1.1f));

            else if(key == sf::Keyboard::Divide && !CTRL_SHIFT_ALT())
                m_SpritePreview.scale(sf::Vector2f(0.9f, 0.9f));

            else if(key == sf::Keyboard::Space)
                switch_between_sub_mode();
        }
        else
        {
            if(key == sf::Keyboard::Numpad3 && !CTRL_SHIFT_ALT())
            {
                this->onSaveButton();

                int index = m_SceneComboBox->GetSelectedItem() + 1;

                if(index > m_SceneManager.getSceneCount()-1)
                    index = 0;

                m_SceneComboBox->SelectItem(index);
                onSceneComboBox();
            }

            else if(key == sf::Keyboard::Numpad1 && !CTRL_SHIFT_ALT())
            {
                this->onSaveButton();

                int index = m_SceneComboBox->GetSelectedItem() - 1;

                if(index < 0)
                    index = m_SceneManager.getSceneCount()-1;

                m_SceneComboBox->SelectItem(index);
                onSceneComboBox();
            }

            //Reinitialization
            if(key == sf::Keyboard::Numpad5 && !CTRL_SHIFT_ALT())
            {
                double x = m_SceneManager.getCameraSetting().defaultPosition_x;
                double y = m_SceneManager.getCameraSetting().defaultPosition_y;
                m_Camera.setPosition(sf::Vector2f(x,y));
                m_Camera.setRotation(m_SceneManager.getCameraSetting().defaultRotation);
                m_Camera.setZoom(m_SceneManager.getCameraSetting().defaultZoom);
            }

            //update default state
            else if(key == sf::Keyboard::Numpad5 && ALT())
            {
                CameraSettings& settings = m_SceneManager.getCameraSetting();

                settings.defaultPosition_x = m_Camera.getPosition().x;
                settings.defaultPosition_y = m_Camera.getPosition().y;
                settings.defaultRotation = m_Camera.getRotation();
                settings.defaultZoom= m_Camera.getZoom();
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void  DevEngine::handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse)
    {
        //Canvas box
        if(m_MouseIsOnRenderCanvas)
        {
            sf::Vector2f canvas_pos = sf::Vector2f(mouse.x - m_RenderCanvas->GetAbsolutePosition().x, mouse.y - m_RenderCanvas->GetAbsolutePosition().y);
            sf::Vector2f world_pos = canvas_to_world(sf::Vector2f(mouse.x, mouse.y), m_RenderCanvas);

            std::string pos = "Canvas: x = " + toString(canvas_pos.x) + "   y = " + toString(canvas_pos.y) + \
                                "  |----|  "
                            "World: x = " + toString(world_pos.x) + "   y = " + toString(world_pos.y) + \
                            "  |----|  "
                            "Camera: x = " + toString(m_Camera.getPosition().x) + "   y = " + toString(m_Camera.getPosition().y);

            m_MousePositionLabel->SetText(pos);
        }


    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_engine()
    {
        //Engine : Left | Center | Right

        ////////////////////////////////////////////////////////////
        //Center : Mouse_Position| Utility_Button | Render_Canvas | Log_Window

        //Mouse_Position
        m_MousePositionLabel = sfg::Label::Create("Mouse Position");
        m_MousePositionLabel->SetId("little_font_size");
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

        black_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_RenderCanvasColor = sf::Color::Black; onColorRadioButton();});
        gray_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_RenderCanvasColor = sf::Color(119, 119, 119); onColorRadioButton();});
        blue_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_RenderCanvasColor = sf::Color(130, 177, 205); onColorRadioButton();});
        white_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){m_RenderCanvasColor = sf::Color::White; onColorRadioButton();});

        m_PlayButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onPlayButton, this));
        m_PauseButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onPauseButton, this));
        m_StepButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onStepButton, this));
        m_ResetButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onResetButton, this));
        m_QuitButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onQuitButton, this));
        m_UndoButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onUndoButton, this));
        m_RedoButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onRedoButton, this));

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
        m_CanvasDefaultView         = m_RenderCanvas->GetView();
        float defaultViewWidth      = m_WinWidth*0.15f*3.65f-11.f;
        float defaultViewHeight     =  m_WinHeight*0.79f-11.f;
        m_CanvasDefaultView.setCenter(defaultViewWidth/2.f, defaultViewHeight/2.f);
        m_CanvasDefaultView.setSize(sf::Vector2f(defaultViewWidth, defaultViewHeight));

        auto render_canvas_window   = sfg::Window::Create();
        render_canvas_window->SetStyle(sfg::Window::Style::BACKGROUND);
        render_canvas_window->SetRequisition(sf::Vector2f(m_WinWidth*0.15f*3.65f, m_WinHeight*0.79f));
        render_canvas_window->Add(m_RenderCanvas);

        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseEnter).Connect([this](){m_MouseIsOnRenderCanvas = true;});
        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseLeave).Connect([this](){m_MouseIsOnRenderCanvas = false;});
        /*
            m_MouseIsOnRenderCanvas
            m_RenderCanvas
            m_CanvasDefaultView
            m_WinWidth
            m_WinHeight
        */


        //Log_Window
        m_LogLabel = sfg::Label::Create("Nero Game Engine --- Log file " + getdate() + " " + getTime() + "\n");

        auto log_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto log_window = sfg::ScrolledWindow::Create();
        log_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_ALWAYS);
        log_window->AddWithViewport(log_window_box);
        log_window->SetRequisition(sf::Vector2f(m_WinWidth*0.15f*3.65f, m_WinHeight*0.15f+4.f));
        log_window_box->Pack(m_LogLabel);

        /*
            m_LogLabel
            m_WinWidth
            m_WinHeight
        */

        //Center_End
        auto center_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        center_box->Pack(m_MousePositionLabel, false);
        center_box->Pack(utility_button_box, false);
        center_box->Pack(render_canvas_window);
        center_box->Pack(log_window);

        ////////////////////////////////////////////////////////////
        //Left : Left_Up_Notebook | Left_Down_Notebook

        //Left_Up_Notebook : Utility
        //Utility
        auto utility_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_utility_box(utility_view_box);

        auto left_up_notebook = sfg::Notebook::Create();
        left_up_notebook->AppendPage(utility_view_box, sfg::Label::Create("Utility"));

        //Left_Down_Notebook : Sprite | Color | Mesh
        //Sprite
        auto sprite_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        auto sprite_window = sfg::ScrolledWindow::Create();
        sprite_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_ALWAYS);
        sprite_window->AddWithViewport(sprite_box);
        sprite_window->SetRequisition(sf::Vector2f(m_WinWidth*0.15f, m_WinHeight*0.54f));
        build_sprite_window(sprite_box);

        sprite_box->GetSignal(sfg::Button::OnMouseLeave).Connect(std::bind(&DevEngine::onSpriteButtonMouseLeave, this));

        //Color
        auto color_Window = sfg::Window::Create();
        color_Window->SetStyle(sfg::Window::Style::BACKGROUND);
        color_Window->SetRequisition(sf::Vector2f(m_WinWidth*0.15f, 0.f));
        build_color_window(color_Window);

        //Mesh
        auto mesh_Window = sfg::Window::Create();
        mesh_Window->SetStyle(sfg::Window::Style::BACKGROUND);
        mesh_Window->SetRequisition(sf::Vector2f(m_WinWidth*0.15f, 0.f));
        build_mesh_window(mesh_Window);

        //
        auto left_down_notebook = sfg::Notebook::Create();
        left_down_notebook->AppendPage(sprite_window, sfg::Label::Create("Sprite"));
        left_down_notebook->AppendPage(color_Window, sfg::Label::Create("Color"));
        left_down_notebook->AppendPage(mesh_Window, sfg::Label::Create("Mesh"));

        /*
            m_WinWidth
            m_WinHeight

            build_utility_view()
            build_sprite_window()
            build_color_window()
            build_mesh_window()

            onSpriteButtonMouseLeave()
        */


        //Left_End
        auto left_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        left_box->Pack(left_up_notebook);
        left_box->Pack(left_down_notebook);

        ////////////////////////////////////////////////////////////
        //Right : Right_Note_Book : Scene
        //Scene : Left_Box | Scene_Setting_Box

        //Left_Box
        //Mesh
        auto mesh_main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_mesh_box(mesh_main_box);

        //Object
        auto object_main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_object_box(object_main_box);

        //Grid
        auto grid_main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_grid_box(grid_main_box);

        //Layer
        auto layer_main_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        build_layer_box(layer_main_box);

        //
        auto scene_left_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        scene_left_box->Pack(mesh_main_box, false);
        scene_left_box->Pack(object_main_box, false);
        scene_left_box->Pack(grid_main_box, false);
        scene_left_box->Pack(layer_main_box, false);


        //Scene_Setting_Box
        auto scene_setting_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        build_scene_setting_box(scene_setting_box);

        //Scene notebook box
        auto scene_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 20.f);
            scene_box->Pack(scene_left_box);
            scene_box->Pack(scene_setting_box);

        //Right_End
        m_EngineRightNotebook = sfg::Notebook::Create();
        m_EngineRightNotebook->AppendPage(scene_box, sfg::Label::Create("Scene"));
        m_EngineRightNotebook->AppendPage(sfg::Button::Create(""), sfg::Label::Create("Help"));

        m_EngineRightNotebook->GetSignal(sfg::Notebook::OnTabChange).Connect(std::bind(&DevEngine::onEngineRightNotebook, this));

        ////////////////////////////////////////////////////////////
        //Main_Box
        auto main_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
            main_box->Pack(left_box);
            main_box->Pack(center_box);
            main_box->Pack(m_EngineRightNotebook);

        m_Desktop.Add(main_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onEngineRightNotebook()
    {
        switch(m_EngineRightNotebook->GetCurrentPage())
        {
            case 0: //SCENE_Mode
            {
                m_EngineMode                    = SCENE;
                m_Current.objectManager         = m_SceneManager.getObjectManager();
                m_Current.meshEditor            = m_SceneManager.getMeshEditor();
                m_Current.undoManager           = m_SceneManager.getUndoManager();

            }break;

            case 1: //Help_Mode
            {
                m_EngineMode                    = HELP;

            }break;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onEngineSubModeButton()
    {
        if(m_ObjectModeRadioButton->IsActive())
        {
            m_EngineSubMode = OBJECT;
            m_Current.objectManager->updateAllLayerAlpha(255);
            //update_engine_undo();
        }

        else if(m_MeshModeRadioButton->IsActive())
        {
            m_EngineSubMode = MESH;
            m_Current.objectManager->updateAllLayerAlpha(100);
            //update_engine_undo();

        }

        else if(m_PlayModeRadioButton->IsActive())
            m_EngineSubMode = PLAY;
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::switch_between_sub_mode()
    {
        if(m_EngineSubMode == OBJECT && !CTRL_SHIFT_ALT())
        {
            set_sub_mode(MESH);
        }

        else if(m_EngineSubMode != OBJECT && !CTRL_SHIFT_ALT())
        {
            set_sub_mode(OBJECT);
        }

        else if(CTRL() && m_EngineSubMode != PLAY)
            set_sub_mode(PLAY);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::set_sub_mode(Engine_SubMode mode)
    {
        switch(mode)
        {
            case OBJECT:
            {
                m_EngineSubMode = OBJECT;
                m_Current.objectManager->updateAllLayerAlpha(255);
                m_ObjectModeRadioButton->SetActive(true);

            }break;

            case MESH:
            {
                m_EngineSubMode = MESH;
                m_Current.objectManager->updateAllLayerAlpha(100);
                m_MeshModeRadioButton->SetActive(true);

            }break;

            case PLAY:
            {
                m_EngineSubMode = PLAY;
                m_PlayModeRadioButton->SetActive(true);

            }break;
        }
    }


    ////////////////////////////////////////////////////////////
    void DevEngine::onPlayButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            m_SceneManager.buildScene();
            set_sub_mode(PLAY);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onPauseButton()
    {
        if(m_EngineMode != SCENE)
            return;

        if(m_EngineSubMode == OBJECT || m_EngineSubMode == PLAY)
            m_SceneManager.getSceneSettings().pause = !m_SceneManager.getSceneSettings().pause;
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onStepButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            m_SceneManager.getSceneSettings().pause         = true;
            m_SceneManager.getSceneSettings().singleStep    = true;
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onResetButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            m_SceneManager.buildScene();
            //m_Current.objectManager->buildScene(m_SceneManager.getRootObject());
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onQuitButton()
    {
        m_Window.close();

        //To do : Save all scenes
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onUndoButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode == PLAY)
            return;

        nlohmann::json undo = m_Current.undoManager->undo();

        if(!undo.empty())
        {
            m_Current.objectManager->loadScene(undo["scene"]);
            updateLayerTab();

            if(m_ObjectModeRadioButton->IsActive())
                m_Current.objectManager->updateAllLayerAlpha(255);
            else if(m_MeshModeRadioButton->IsActive())
                m_Current.objectManager->updateAllLayerAlpha(100);
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onRedoButton()
    {

        if(m_EngineMode != SCENE || m_EngineSubMode == PLAY)
            return;

        nlohmann::json redo = m_Current.undoManager->redo();

        if(!redo.empty())
        {
            m_Current.objectManager->loadScene(redo["scene"]);
            updateLayerTab();

            if(m_ObjectModeRadioButton->IsActive())
                m_Current.objectManager->updateAllLayerAlpha(255);
            else if(m_MeshModeRadioButton->IsActive())
                m_Current.objectManager->updateAllLayerAlpha(100);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_utility_box(sfg::Box::Ptr main_box)
    {
        //
        m_ObjectModeRadioButton = sfg::RadioButton::Create("Object");
        m_MeshModeRadioButton   = sfg::RadioButton::Create("Mesh",  m_ObjectModeRadioButton->GetGroup());
        m_PlayModeRadioButton   = sfg::RadioButton::Create("Play",  m_ObjectModeRadioButton->GetGroup());

        m_SaveButton            = sfg::Button::Create("Save");
        m_LoadButton            = sfg::Button::Create("Laod");

        m_AutoSaveChekButton = sfg::CheckButton::Create("Auto save");

        //
        m_ObjectModeRadioButton->SetActive(true);
        m_AutoSaveChekButton->SetActive(m_AutoSave);

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
        m_ObjectModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onEngineSubModeButton, this));
        m_MeshModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onEngineSubModeButton, this));
        m_PlayModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onEngineSubModeButton, this));

        m_SaveButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onSaveButton, this));
        m_LoadButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onLoadButton, this));

        m_AutoSaveChekButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect([this](){m_AutoSave = m_AutoSaveChekButton->IsActive();});
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onSaveButton()
    {
        //update layers order
        m_Current.objectManager->updateLayerOrder();

        //
        std::string scene_name = m_SceneManager.getSceneName();
        std::string path = NERO_FOLDER + "/" + scene_name + "/" + scene_name +  ".json";

        nlohmann::json scene = m_Current.objectManager->saveScene();
        scene["name"] = scene_name;

        std::ofstream stream(path);

        if(!stream.is_open())
            return;

        stream << scene.dump(3);
        stream.close();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onLoadButton()
    {
        std::string scene_name = m_SceneComboBox->GetSelectedText();
        std::string path = NERO_FOLDER + "/" + scene_name + "/" + scene_name +  ".json";

        using namespace boost::filesystem;

        if(!exists(path))
            return;

        std::ifstream stream(path);

        if(!stream.is_open())
            return;

        nlohmann::json scene;

        stream >> scene;
        stream.close();

        m_Current.objectManager->loadScene(scene);

        updateLayerTab();

        update_engine_undo();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_sprite_window(sfg::Box::Ptr sprite_box)
    {
        const std::vector<std::string>& spriteTab = m_ResourceManager.Texture.getSpriteTab();
        int sprite_count = spriteTab.size();


        int nbr = (sprite_count%2 == 0) ? sprite_count : sprite_count-1;

        for(int i = 0; i < nbr; i = i+2)
        {
            auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);

            for(int j = 0; j < 2; j++) //0 and 1
            {
                sfg::Button::Ptr button = sfg::Button::Create(spriteTab[i+j]);
                box->Pack(button);

                button->GetSignal(sfg::Button::OnLeftClick).Connect(onSpriteButton(spriteTab[i+j]));
                button->GetSignal(sfg::Button::OnMouseEnter).Connect(onSpriteButtonMouseEnter(spriteTab[i+j]));
            }

            sprite_box->Pack(box);
        }

        if(sprite_count%2 != 0)
        {
            sfg::Button::Ptr button = sfg::Button::Create(spriteTab[sprite_count-1]);
            sprite_box->Pack(button);

            button->GetSignal(sfg::Button::OnLeftClick).Connect(onSpriteButton(spriteTab[sprite_count-1]));
            button->GetSignal(sfg::Button::OnMouseEnter).Connect(onSpriteButtonMouseEnter(spriteTab[sprite_count-1]));
        }
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngine::onSpriteButton(const std::string &label)
    {
        return [&]()
        {
            if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

            sf::IntRect rect            = m_ResourceManager.Texture.getSpriteRect(label);
            sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
            sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

            if(CTRL())
            {
                if(m_Current.objectManager->addObject(Object::Meshed_Object, label, world_pos))
                    updateLayerTab();
            }
            else
            {
                if(m_Current.objectManager->addObject(Object::Sprite_Object, label, world_pos))
                    updateLayerTab();
            }
        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngine::onSpriteButtonMouseEnter(const std::string &label)
    {
        return [&]()
            {
                m_TexturePreview = m_ResourceManager.Texture.getSpriteTexture(label);
                sf::IntRect rect = m_ResourceManager.Texture.getSpriteRect(label);
                m_SpritePreview.setTextureRect(rect);
                m_SpritePreview.setTexture(m_TexturePreview);
                m_SpritePreview.setOrigin(rect.width/2.f, rect.height/2.f);
                m_SpritePreview.setPosition(m_CanvasDefaultView.getSize().x/2.f, rect.height/2.f + 100.f);
                m_SpritePreview.setColor(sf::Color(255, 255, 255, 100));
//                m_SpritePreview.setScale(sf::Vector2f(1.f, 1.3f));
            };
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onSpriteButtonMouseLeave()
    {
        m_SpritePreview = sf::Sprite();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_color_window(sfg::Window::Ptr color_window)
    {

        m_SpriteColorRadioButton    = sfg::RadioButton::Create("Sprite");
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

        alpha_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        red_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        green_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        blue_scale->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});


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

        alpha_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        red_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        green_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});
        blue_spin->GetSignal(sfg::Scale::OnMouseLeftRelease).Connect([this](){update_engine_undo();});

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

        m_AlphaAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngine::onColorAdjustment, this));
        m_RedAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngine::onColorAdjustment, this));
        m_GreenAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngine::onColorAdjustment, this));
        m_BlueAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngine::onColorAdjustment, this));

        m_SpriteColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onColorRadioButton, this));
        m_LayerColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onColorRadioButton, this));
        m_CanvasColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onColorRadioButton, this));

    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onColorRadioButton()
    {
        if(m_SpriteColorRadioButton->IsActive())
        {
            Object::Ptr object = m_Current.objectManager->getSelectedObject();

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
            /*sf::Color c = m_Current.objectManager->getLayerColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);*/

        }
        else if (m_CanvasColorRadioButton->IsActive())
        {
            sf::Color c = m_RenderCanvasColor;

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onColorAdjustment()
    {
        sf::Color color(m_RedAdjustment->GetValue(), m_GreenAdjustment->GetValue(), m_BlueAdjustment->GetValue(), m_AlphaAdjustment->GetValue());

        if(m_LayerColorRadioButton->IsActive())
            m_Current.objectManager->updateLayerColor(color);
        else if(m_SpriteColorRadioButton->IsActive())
            m_Current.objectManager->updateSpriteColor(color);
        else if (m_CanvasColorRadioButton->IsActive())
            m_RenderCanvasColor = color;

    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_mesh_window(sfg::Window::Ptr mesh_window)
    {
        m_MeshFixedRotationCheckButton      = sfg::CheckButton::Create("Fixed Rotation");
        m_MeshIsSensorCheckButton           = sfg::CheckButton::Create("Is Sensor");
        m_MeshAllowSleepCheckButton         = sfg::CheckButton::Create("Allow Sleep");

        m_MeshDensitySpinButton             = sfg::SpinButton::Create(0.f, 100.f, 1.f);
        m_MeshFrictionSpinButton            = sfg::SpinButton::Create(0.f, 10.f, 1.f);
        m_MeshRestitutionSpinButton         = sfg::SpinButton::Create(0.f, 10.f, 1.f);
        m_MeshGravityScaleSpinButton        = sfg::SpinButton::Create(0.f, 10.f, 1.f);


        m_MeshDensitySpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));

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

        m_MeshFixedRotationCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngine::onMeshFixedRotationButton, this));
        m_MeshIsSensorCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngine::onMeshIsSensorButton, this));
        m_MeshAllowSleepCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngine::onMeshAllowSleepButton, this));

        m_MeshDensitySpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onMeshDensityButton, this));
        m_MeshFrictionSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onMeshFrictionButton, this));
        m_MeshRestitutionSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onMeshRestitutionButton, this));
        m_MeshGravityScaleSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onMeshGravityScaleButton, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshFixedRotationButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshFixedRotation(m_MeshFixedRotationCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshIsSensorButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshSensor(m_MeshIsSensorCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshAllowSleepButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshAllowSleep(m_MeshAllowSleepCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshDensityButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshDensity(m_MeshDensitySpinButton->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshFrictionButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshFriction(m_MeshFrictionSpinButton->GetValue()/10.f);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshRestitutionButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshRestitution(m_MeshRestitutionSpinButton->GetValue()/10.f);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshGravityScaleButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshGravityScale(m_MeshGravityScaleSpinButton->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_scene_setting_box(sfg::Box::Ptr main_box)
    {
        m_SceneComboBox = sfg::ComboBox::Create();

        m_Gravity_x = sfg::SpinButton::Create(-50.f, 50.f, 1.f);
        m_Gravity_x->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_Gravity_y = sfg::SpinButton::Create(-50.f, 50.f, 1.f);
        m_Gravity_y->SetRequisition( sf::Vector2f(50.f, 0.f ));


        m_VelocityIterationSpinButton = sfg::SpinButton::Create(1.f, 500.f, 6.f);
        m_VelocityIterationSpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_PositionIterationSpinButton = sfg::SpinButton::Create(0.f, 100.f, 2.f);
        m_PositionIterationSpinButton->SetRequisition( sf::Vector2f(50.f, 0.f ));

        m_HertzSpinButton = sfg::SpinButton::Create(5.0f, 200.f, 2.f);
        m_HertzSpinButton->SetRequisition( sf::Vector2f(49.f, 0.f ));

        auto spin_table = sfg::Table::Create();
        spin_table->Attach(sfg::Label::Create("Grav X"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_Gravity_x, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Grav Y"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_Gravity_y, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Vel Itr"), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_VelocityIterationSpinButton, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Pos Itr "), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_PositionIterationSpinButton, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(sfg::Label::Create("Hertz"), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        spin_table->Attach(m_HertzSpinButton, sf::Rect<sf::Uint32>( 1, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );


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

        auto draw_frame = sfg::Frame::Create("Draw");
        auto frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        frame_box->Pack(m_DrawAxisCheckButton);
        frame_box->Pack(m_DrawGridCheckButton);
        frame_box->Pack(m_DrawShapesCheckButton);
        frame_box->Pack(m_DrawJointsCheckButton);
        frame_box->Pack(m_DrawAABBsCheckButton);
        frame_box->Pack(m_DrawContactPointsCheckButton);
        frame_box->Pack(m_DrawContactNormalsCheckButton);
        frame_box->Pack(m_DrawContactImpulsesCheckButton);
        frame_box->Pack(m_DrawFrictionImpulsesCheckButton);
        frame_box->Pack(m_DrawCenterOfMassesCheckButton);
        frame_box->Pack(m_DrawStatisticsCheckButton);
        frame_box->Pack(m_DrawProfileCheckButton);
        draw_frame->Add(frame_box);

        main_box->Pack(sfg::Label::Create("Scene"), true, false);
        main_box->Pack(m_SceneComboBox, true, false);
        main_box->Pack(spin_table, true, false);
        main_box->Pack(m_SleepCheckButton, true, false);
        main_box->Pack(m_WarmStartingCheckButton, true, false);
        main_box->Pack(m_TimeOfImpactCheckButton, true, false);
        main_box->Pack(m_SubSteppingCheckButton, true, false);
        main_box->Pack(draw_frame, true, false);

        //
        m_VelocityIterationSpinButton->SetValue(m_SceneManager.getSceneSettings().velocityIterations);
        m_PositionIterationSpinButton->SetValue(m_SceneManager.getSceneSettings().positionIterations);
        m_HertzSpinButton->SetValue(m_SceneManager.getSceneSettings().hz);

        m_SleepCheckButton->SetActive(m_SceneManager.getSceneSettings().enableSleep );
        m_WarmStartingCheckButton->SetActive(m_SceneManager.getSceneSettings().enableWarmStarting);
        m_TimeOfImpactCheckButton->SetActive(m_SceneManager.getSceneSettings().enableContinuous);
        m_SubSteppingCheckButton->SetActive(m_SceneManager.getSceneSettings().enableSubStepping);

        m_DrawShapesCheckButton->SetActive( m_SceneManager.getSceneSettings().drawShapes);
        m_DrawJointsCheckButton->SetActive(m_SceneManager.getSceneSettings().drawJoints);
        m_DrawAABBsCheckButton->SetActive(m_SceneManager.getSceneSettings().drawAABBs);
        m_DrawContactPointsCheckButton->SetActive(m_SceneManager.getSceneSettings().drawContactPoints);
        m_DrawContactNormalsCheckButton->SetActive(m_SceneManager.getSceneSettings().drawContactNormals);
        m_DrawContactImpulsesCheckButton->SetActive(m_SceneManager.getSceneSettings().drawContactImpulse);
        m_DrawFrictionImpulsesCheckButton->SetActive(m_SceneManager.getSceneSettings().drawFrictionImpulse);
        m_DrawCenterOfMassesCheckButton->SetActive(m_SceneManager.getSceneSettings().drawCOMs);
        m_DrawStatisticsCheckButton->SetActive(m_SceneManager.getSceneSettings().drawStats);
        m_DrawProfileCheckButton->SetActive(m_SceneManager.getSceneSettings().drawProfile);

        ////////////////////////////////////////////////////////////

        m_SceneComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngine::onSceneComboBox, this));

        m_VelocityIterationSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onScenePinButton, this));
        m_PositionIterationSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onScenePinButton, this));
        m_HertzSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngine::onScenePinButton, this));

        m_SleepCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_WarmStartingCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_TimeOfImpactCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_SubSteppingCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));

        m_DrawShapesCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawJointsCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawAABBsCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawContactPointsCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawContactNormalsCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawContactImpulsesCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawFrictionImpulsesCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawCenterOfMassesCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawStatisticsCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
        m_DrawProfileCheckButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngine::onSceneCheckButton, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onSceneComboBox()
    {
        if(m_EngineMode != Engine_Mode::SCENE || m_EngineSubMode != Engine_SubMode::OBJECT)
            return;

        //Select a scene
        sf::String scene_title = m_SceneComboBox->GetSelectedText();
        m_SceneManager.setScene(scene_title);

        //Update CurrentScene
        m_Current.objectManager         = m_SceneManager.getObjectManager();
        m_Current.meshEditor            = m_SceneManager.getMeshEditor();
        m_Current.undoManager           = m_SceneManager.getUndoManager();

        //Set up the camera
        sf::Vector2f pos;
        pos.x = m_SceneManager.getCameraSetting().position_x;
        pos.y = m_SceneManager.getCameraSetting().position_y;
        m_Camera.setPosition(pos);
        m_Camera.setRotation(m_SceneManager.getCameraSetting().rotation);
        m_Camera.setZoom(m_SceneManager.getCameraSetting().zoom);

        updateLayerTab();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onScenePinButton()
    {
        m_SceneManager.getSceneSettings().velocityIterations  =  m_VelocityIterationSpinButton->GetValue();
        m_SceneManager.getSceneSettings().positionIterations  =  m_PositionIterationSpinButton->GetValue();
        m_SceneManager.getSceneSettings().hz                  =  m_HertzSpinButton->GetValue();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onSceneCheckButton()
    {
        m_SceneManager.getSceneSettings().enableSleep           = m_SleepCheckButton->IsActive();
        m_SceneManager.getSceneSettings().enableWarmStarting    = m_WarmStartingCheckButton->IsActive();
        m_SceneManager.getSceneSettings().enableContinuous      = m_TimeOfImpactCheckButton->IsActive();
        m_SceneManager.getSceneSettings().enableSubStepping     = m_SubSteppingCheckButton->IsActive();

        m_SceneManager.getSceneSettings().drawShapes            = m_DrawShapesCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawJoints            = m_DrawJointsCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawAABBs             = m_DrawAABBsCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawContactPoints     = m_DrawContactPointsCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawContactNormals    = m_DrawContactNormalsCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawContactImpulse    = m_DrawContactImpulsesCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawFrictionImpulse   = m_DrawFrictionImpulsesCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawCOMs              = m_DrawCenterOfMassesCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawStats             = m_DrawStatisticsCheckButton->IsActive();
        m_SceneManager.getSceneSettings().drawProfile           = m_DrawProfileCheckButton->IsActive();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_mesh_box(sfg::Box::Ptr main_box)
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

        m_MeshComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngine::onMeshComboBox, this));
        m_MeshTypeComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngine::onMeshTypeComboBox, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshComboBox()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
            sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

            if(m_Current.objectManager->addObject(Object::Mesh_Object, m_MeshComboBox->GetSelectedText(), world_pos))
                updateLayerTab();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onMeshTypeComboBox()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setMeshType(m_MeshTypeComboBox->GetSelectedText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_object_box(sfg::Box::Ptr main_box)
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

        m_ObjectNameEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngine::onObjectNameEntry, this));
        m_ObjectCategoryEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngine::onObjectCategoryEntry, this));

        m_ObjectNameEntry->GetSignal(sfg::Entry::OnMouseLeave).Connect([this](){m_RenderCanvas->GrabFocus();});
        m_ObjectCategoryEntry->GetSignal(sfg::Entry::OnMouseLeave).Connect([this](){m_RenderCanvas->GrabFocus();});
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onObjectNameEntry()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setObjectName(m_ObjectNameEntry->GetText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onObjectCategoryEntry()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_Current.objectManager->setObjectCategory(m_ObjectCategoryEntry->GetText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_grid_box(sfg::Box::Ptr main_box)
    {
        m_Grid_x_SpinButton         =   sfg::SpinButton::Create(5.f, 100.f, 1.f);
        m_Grid_y_SpinButton         =   sfg::SpinButton::Create(5.f, 100.f, 1.f);

        m_Grid_x_SpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_Grid_y_SpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));

        m_Grid_x_SpinButton->SetId("layer");
        m_Grid_y_SpinButton->SetId("layer");


        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Width"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_Grid_x_SpinButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Height"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_Grid_y_SpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto frame = sfg::Frame::Create("Grid");
        frame->Add(table);

        main_box->Pack(frame, false);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::build_layer_box(sfg::Box::Ptr main_box)
    {
        //Create Buttons
        m_AddLayerButton        = sfg::Button::Create("Add");
        m_DeleteLayerButton     = sfg::Button::Create("Del");
        m_UpLayerButton         = sfg::Button::Create("Up");
        m_DownLayerButton       = sfg::Button::Create("Down");

        //Create the layers box
        m_LayerBox              = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);

        //Create the layers scrolling window
        auto window = sfg::ScrolledWindow::Create();
        window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
        window->AddWithViewport(m_LayerBox);
        window->SetRequisition(sf::Vector2f(1100.f*0.17f, 576.f*0.5f));

        //Pack buttons
        auto button_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
            button_box->Pack(m_AddLayerButton);
            button_box->Pack(m_DeleteLayerButton);
            button_box->Pack(m_UpLayerButton);
            button_box->Pack(m_DownLayerButton);

        //Pack all
        auto layer_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
            layer_box->Pack(button_box, false);
            layer_box->Pack(window);

        //Main Box
        auto frame = sfg::Frame::Create("Layer");
        frame->Add(layer_box);

        main_box->Pack(frame, false);

        //Signals and Slots
        m_AddLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onAddLayerButton, this));
        m_DeleteLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onDeleteLayerButton, this));
        m_UpLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onUpLayerButton, this));
        m_DownLayerButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngine::onDownLayerButton, this));
    }


    ////////////////////////////////////////////////////////////
    DevEngine::Layer::Layer()
    {
        seeToggleButton     = sfg::ToggleButton::Create("show");
        selectRadioButton   = sfg::RadioButton::Create("");
        nameEntry           = sfg::Entry::Create();

        nameEntry->SetRequisition(sf::Vector2f(100.f, 0.f));
    }

    ////////////////////////////////////////////////////////////
    sfg::Box::Ptr DevEngine::Layer::get()
    {
        auto box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
        box->Pack(seeToggleButton, false);
        box->Pack(selectRadioButton, false);
        box->Pack(nameEntry);

        return box;
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onAddLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        if(m_LayerTab.size() < SPRITE_LAYER_MAX)
        {
            auto layer = m_Current.objectManager->addLayer();
            createLayer(*layer);
            updateLayerBox();

        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onDeleteLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        if(m_LayerTab.size() == 1)
        {
            m_Current.objectManager->deleteLayer(m_LayerTab.back().id);
            m_LayerTab.pop_back();
            onAddLayerButton();
        }
        else
        {
            for(auto it =  m_LayerTab.begin(); it !=  m_LayerTab.end(); it++)
            {
                if(it->selectRadioButton->IsActive())
                {
                    if(it != m_LayerTab.begin())
                        (it-1)->selectRadioButton->SetActive(true);
                    else if(it == m_LayerTab.begin() && m_LayerTab.size() != 1)
                        (it+1)->selectRadioButton->SetActive(true);

                    m_Current.objectManager->deleteLayer(it->id);

                    m_LayerTab.erase(it);

                    break;
                }
            }
        }

        updateLayerBox();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::onUpLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != (m_LayerTab.end()-1))
            {
                 m_Current.objectManager->moveLayerUp(it->id);

                std::iter_swap(it, it++);
            }
        }

        updateLayerBox();
    }


    ////////////////////////////////////////////////////////////
    void DevEngine::onDownLayerButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
            return;

        for(auto it = m_LayerTab.begin(); it != m_LayerTab.end(); it++)
        {
            if(it->selectRadioButton->IsActive() && it != m_LayerTab.begin())
            {
                 m_Current.objectManager->moveLayerDown(it->id);

                std::iter_swap(it, it--);
            }
        }

        updateLayerBox();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::updateLayerTab()
    {
        auto layerTab = m_Current.objectManager->getLayerTab();
        m_LayerTab.clear();

        for(auto layer : layerTab)
            createLayer(*layer);

        updateLayerBox();
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::createLayer(const LayerObject& layerObject)
    {
        Layer layer;
        layer.id = layerObject.getId();
        if(!m_LayerTab.empty())
            layer.selectRadioButton->SetGroup(m_LayerTab[0].selectRadioButton->GetGroup());
        layer.selectRadioButton->SetActive(layerObject.isSelected());
        layer.seeToggleButton->SetActive(layerObject.isVisible());
        layer.nameEntry->SetText(layerObject.getName());

            switch(layerObject.getSecondType())
            {
                case Object::Sprite_Object: layer.nameEntry->SetId("sprite_layer"); break;
                case Object::Mesh_Object: layer.nameEntry->SetId("mesh_layer"); break;
                case Object::Meshed_Object: layer.nameEntry->SetId("meshed_layer"); break;
            }

            m_LayerTab.push_back(layer);
            m_LayerTab.back().seeToggleButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(onSeeLayerButton(layerObject.getId()));
            m_LayerTab.back().selectRadioButton->GetSignal(sfg::RadioButton::OnMouseLeftPress).Connect(onSelectLayerButton(layerObject.getId()));
            m_LayerTab.back().nameEntry->GetSignal(sfg::Entry::OnKeyRelease).Connect(onLayerNameEntry(layerObject.getId()));
    }


    ////////////////////////////////////////////////////////////
    void DevEngine::updateLayerBox()
    {
        m_LayerBox->RemoveAll();

        for(auto it = m_LayerTab.rbegin(); it != m_LayerTab.rend(); it++)
            m_LayerBox->Pack(it->get(), false);
    }


    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngine::onSeeLayerButton(const int& id)
    {
        return [this, id]()
        {
             m_Current.objectManager->updateLayerVisibility(id);

        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngine::onSelectLayerButton(const int& id)
    {
        return [this, id]()
        {
            m_Current.objectManager->selectLayer(id);
        };
    }

    ////////////////////////////////////////////////////////////
    std::function<void ()> DevEngine::onLayerNameEntry(const int& id)
    {
        return [this, id]()
        {
            for(auto layer : m_LayerTab)
                if(layer.id == id)
                    m_Current.objectManager->updateLayerName(id, layer.nameEntry->GetText());
        };
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::update_engine()
    {
        Object::Ptr object = m_Current.objectManager->getSelectedObject();

        m_ObjectNameEntry->SetText(object->getName());
        m_ObjectCategoryEntry->SetText(object->getCategory());

        if(object->getSecondType() == Object::Sprite_Object && m_SpriteColorRadioButton->IsActive())
        {
            sf::Color c = object->getColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }
        else if (object->getSecondType() == Object::Mesh_Object || object->getSecondType() == Object::Meshed_Object)
        {
            MeshObject::Ptr mesh_object;

            if(object->getSecondType() == Object::Mesh_Object)
                mesh_object = std::static_pointer_cast<MeshObject>(object);
            else
                mesh_object = std::static_pointer_cast<MeshObject>(object->getChild(0));

            m_MeshFixedRotationCheckButton->SetActive(mesh_object->getMeshFixedRotation());
            m_MeshIsSensorCheckButton->SetActive(mesh_object->getMeshSensor());
            m_MeshAllowSleepCheckButton->SetActive(mesh_object->getMeshAllowSleep());

            m_MeshDensitySpinButton->SetValue(mesh_object->getMeshDensity());
            m_MeshFrictionSpinButton->SetValue(mesh_object->getMeshFriction()*10);
            m_MeshRestitutionSpinButton->SetValue(mesh_object->getMeshRestitution()*10);
            m_MeshGravityScaleSpinButton->SetValue(mesh_object->getMeshGravityScale());
        }
    }


    ////////////////////////////////////////////////////////////
    void DevEngine::update_engine_undo()
    {
        /*nlohmann::json undo;
        undo["scene"] = m_Current.objectManager->save();
        undo["engine_sub_mode"] = m_EngineSubMode;
        undo["update_layer_tab"] = true;

        m_Current.undoManager->add(undo);*/

        nlohmann::json scene = m_Current.objectManager->saveScene();
        nlohmann::json undo;
        undo["scene"] = scene;
        undo["engine_sub_mode"] = m_EngineSubMode;
        undo["update_layer_tab"] = true;
        m_Current.undoManager->add(undo);
    }

    ////////////////////////////////////////////////////////////
    void DevEngine::update_engine_undo_layer()
    {
        nlohmann::json undo;
        undo["scene"] = m_Current.objectManager->saveScene();
        undo["engine_sub_mode"] = m_EngineSubMode;
        undo["update_layer_tab"] = true;

        m_Current.undoManager->add(undo);
    }

}
