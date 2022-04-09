////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//STD
#include <windows.h>
#include <stdio.h>
//NERO
#include <Nero/engine/DevEngineUI.h>
#include <Nero/utility/Utility.h>
//SFGUI
#include <SFGUI/Separator.hpp>
//BOOST
#include <boost/filesystem.hpp>
//STD
#include <string>
//IMGUI
bool IMGUI_COLOR_PICKER_OPEN;
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    DevEngineUI::DevEngineUI(sf::RenderWindow& window):
         m_Sfgui()
        ,m_Window(window)
        ,m_EngineMode(SCENE)
        ,m_EngineSubMode(OBJECT)
        ,m_SceneManager(nullptr)
        ,m_ActiveSceneBuilder(nullptr)
        ,m_ActiveMeshEditor(nullptr)
        ,m_ActiveUndoManager(nullptr)
        ,m_ActiveSoundManager(nullptr)
        ,m_ActiveGrid(nullptr)
        ,m_Camera(nullptr)
        ,m_CanvasColor(sf::Color::Black)
        ,m_MousePositionText("")
        ,m_EngineSetting()
        ,m_FrontScreen(false)
        ,m_CurrentView("World View")
        ,m_SceneRenderer(nullptr)
        ,m_RenderScene(false)
        ,m_ColorPickerColor(1.0f,1.0f,1.0f,1.0f)
        ,m_ColorPickerLastColor(1.0f,1.0f,1.0f,1.0f)
        ,m_RenderColorPicker(true)
        ,m_RendererActive(false)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleEvent(sf::Event& event)
    {
        if(m_RendererActive)
        {
            if(event.type == sf::Event::Closed)
            {
                onQuitButton();
            }

            return;
        }

        //IMGUI
        if(m_RenderColorPicker)
        {
            ImGui::SFML::ProcessEvent(event);
        }

        //UI
        m_Desktop.HandleEvent(event);

        //Scene Builder
        if(m_MouseOnCanvas && m_EngineMode == SCENE && m_EngineSubMode == OBJECT && !IMGUI_COLOR_PICKER_OPEN)
            m_ActiveSceneBuilder->handleEvent(event);

        //Mesh Editor
        if(m_MouseOnCanvas && m_EngineMode == SCENE && m_EngineSubMode == MESH&& !IMGUI_COLOR_PICKER_OPEN )
            m_ActiveMeshEditor->handleEvent(event);

        //Scene Manager
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY && !IMGUI_COLOR_PICKER_OPEN)
        {
            try
            {
                m_SceneManager->handleEvent(event);
            }
            catch(std::exception const& e)
            {
                updateLog(e.what());
                onResetButton();
                setEngineSubMode(OBJECT);
            }
        }


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

            //Mouse button event
            case sf::Event::MouseButtonPressed:
                handleMouseButtonInput(event.mouseButton, true);
                break;
            case sf::Event::MouseButtonReleased:
                handleMouseButtonInput(event.mouseButton, false);
                break;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::update(const sf::Time& timeStep)
    {
        if(m_RendererActive)
            return;

        //UI
        m_Desktop.Update(timeStep.asSeconds());

        //Camera
        m_Camera->update(timeStep);
        m_RenderCanvas->SetView(m_Camera->getView());

        //Scene Builder
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->update(timeStep);

        //Scene Manager
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            try
            {
               m_SceneManager->update(timeStep);
            }
            catch(std::exception const& e)
            {
                updateLog(e.what());
            }
        }

        //Update the current Scene camera Setting
        updateCameraSetting();

        //Auto save
        if(m_EngineSetting.autoSave)
            autoSave();

        //Position Label
        updatePositionLabel();

        //Frame Rate
        updateInfo();

        //Color Picker
        if(m_ColorPickerColor.x != m_ColorPickerLastColor.x ||
           m_ColorPickerColor.y != m_ColorPickerLastColor.y ||
           m_ColorPickerColor.w != m_ColorPickerLastColor.w ||
           m_ColorPickerColor.z != m_ColorPickerLastColor.z)
        {
            m_RedAdjustment->SetValue(m_ColorPickerColor.x*255);
            m_GreenAdjustment->SetValue(m_ColorPickerColor.y*255);
            m_BlueAdjustment->SetValue(m_ColorPickerColor.z*255);
            m_AlphaAdjustment->SetValue(m_ColorPickerColor.w*255);

            m_ColorPickerLastColor = m_ColorPickerColor;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::render()
    {
        if(m_RendererActive) return;

        //Render Canvas
        m_RenderCanvas->Clear(m_CanvasColor);

            //Render the Grid on Front Screen
            if(m_SceneManager->getSceneSetting().drawGrid)
            {
                m_RenderCanvas->Draw(*m_ActiveGrid);
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
                        m_SceneManager->renderFrontScreen();

                if(m_SceneManager->getSceneSetting().drawAxis)
                {
                    m_Camera->render();
                    m_RenderCanvas->Draw(m_InfoText);
                }

                m_RenderCanvas->Draw(m_Preview);


            m_RenderCanvas->SetView(m_Camera->getView());

        m_RenderCanvas->Display();

        //UI
        m_Sfgui.Display(m_Window);

        //Render Scene
        if(m_RenderScene && !m_SceneRenderer)
        {
            m_RenderScene = false;
            m_RendererActive = true;
            onRenderButton();
        }

        if(m_RenderColorPicker)
        {
            renderColorPicker();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::buildInterface()
    {
        //UI properties
        m_Desktop.SetProperty("Label#little_font_size", "FontSize", 11.f);
        m_Desktop.SetProperty("Entry#sprite_layer", "BackgroundColor", sf::Color(143, 130, 114));
        m_Desktop.SetProperty("Entry#mesh_layer", "BackgroundColor", sf::Color(53, 80, 57));
        m_Desktop.SetProperty("Entry#meshed_layer", "BackgroundColor", sf::Color(53, 85, 101));
        m_Desktop.SetProperty("Entry#animation_layer", "BackgroundColor", sf::Color(218, 77, 77));
        m_Desktop.SetProperty("Entry#animation_meshed_layer", "BackgroundColor", sf::Color(135, 104, 110));
        m_Desktop.SetProperty("Entry#button_layer", "BackgroundColor", sf::Color(128,10,202));
        m_Desktop.SetProperty("Entry#text_layer", "BackgroundColor", sf::Color(34, 123, 108));
        m_Desktop.SetProperty("Entry#grid_entry" , "FontSize", 8.f);
        m_Desktop.SetProperty("Entry#little_font_size" , "FontSize", 10.f);
        m_Desktop.SetProperty("SpinButton#little_font_size" , "FontSize", 10.f);
        m_Desktop.SetProperty("Notebook#layer_note_book", "BorderWidth", 0.f);
        m_Desktop.SetProperty("Notebook#layer_note_book", "Padding", 0.f);
        m_Desktop.SetProperty("Frame#grid_frame", "Padding", 2.f);
        m_Desktop.SetProperty("ScrolledWindow#help_window", "BorderColor", sf::Color::Transparent);

        //Build the UI
        auto main_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
        buildLeft(main_box);
        buildCenter(main_box);
        buildRight(main_box);
        m_Desktop.Add(main_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setup()
    {
        //Scene list
        auto sceneTable = m_SceneManager->getSceneTable();
        for(std::string scene : sceneTable)
        {
            m_SceneComboBox->AppendItem(scene);
        }

        //Music list
        auto soundTable = m_ResourceManager->sound.getSoundBufferTable();
        for(std::string sound : soundTable)
        {
            m_SoundComboBox->AppendItem(sound);
        }

        auto musicTable =  m_ResourceManager->music.getMusicTable();
        for(std::string music : musicTable)
        {
            m_MusicComboBox->AppendItem(music);
        }

        m_InfoText.setFont(m_ResourceManager->font.getDefaultFont());
        m_InfoText.setCharacterSize(13.f);
        m_InfoText.setFillColor(sf::Color::White);

        float width = m_Window.getSize().x - 1305.f * 0.14f - 412.f - 230.f;
        float height = m_Window.getSize().y * 0.75f - 35.f;
        m_InfoText.setPosition(sf::Vector2f(width, height));

        m_ObjectModeRadioButton->SetActive(true);
        m_AutoSaveChekButton->SetActive(m_EngineSetting.autoSave);
    }


    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed)
    {
        if(isPressed)
        {
            if(key == sf::Keyboard::Return && !CTRL_SHIFT_ALT() && m_MouseOnCanvas && m_EngineSubMode == OBJECT)
                onPlayButton();

            else if(key == sf::Keyboard::Return && !CTRL_SHIFT_ALT() && m_MouseOnCanvas && m_EngineSubMode == PLAY)
                onPauseButton();

            else if(key == sf::Keyboard::Return && CTRL() && m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            {
                m_RenderScene = true;
            }

            else if(key == sf::Keyboard::Up && CTRL())
                onPauseButton();

            else if(key == sf::Keyboard::Right && CTRL())
                onStepButton();

            else if(key == sf::Keyboard::Left && CTRL())
                onResetButton();

            else if(key == sf::Keyboard::Z && CTRL())
                onUndoButton();

            else if(key == sf::Keyboard::Y && CTRL())
                onRedoButton();

            else if(key == sf::Keyboard::S && CTRL())
                onSaveButton();

            else if(key == sf::Keyboard::L && CTRL())
                onLoadButton();

            else if(key == sf::Keyboard::Space && m_MouseOnCanvas && !m_FrontScreen)
                switchEngineSubMode();
        }
        else
        {
            if(key == sf::Keyboard::PageDown && !CTRL_SHIFT_ALT())
            {
                setEngineSubMode(OBJECT);

                onSaveButton();

                int index = m_SceneComboBox->GetSelectedItem() + 1;
                if(index > m_SceneManager->getSceneCount()-1)
                    index = 0;

                m_SceneComboBox->SelectItem(index);

                onSceneSelection();
            }

            else if(key == sf::Keyboard::PageUp && !CTRL_SHIFT_ALT())
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
            else if(key == sf::Keyboard::Numpad5 && ALT() && m_MouseOnCanvas)
            {
                updateLog("saving camera state");

                CameraSetting& setting = m_FrontScreen ? m_SceneManager->getScreenCameraSetting() : m_SceneManager->getCameraSetting();

                setting.defaultPosition = m_Camera->getPosition();
                setting.defaultRotation = m_Camera->getRotation();
                setting.defaultZoom     = m_Camera->getZoom();
            }

            //Rest Camera to default state
            if(key == sf::Keyboard::Numpad5 && !CTRL_SHIFT_ALT() && m_MouseOnCanvas)
            {
                updateLog("loading camera state");

                CameraSetting& setting = m_FrontScreen ? m_SceneManager->getScreenCameraSetting() : m_SceneManager->getCameraSetting();

                m_Camera->setPosition(setting.defaultPosition);
                m_Camera->setRotation(setting.defaultRotation);
                m_Camera->setZoom(setting.defaultZoom);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed)
    {
        if(isPressed)
        {
            if(mouse.button == sf::Mouse::Middle && m_MouseOnCanvas)
            {
                if (ALT())
                {
                    updateLog("saving camera state");

                    CameraSetting& setting = m_FrontScreen ? m_SceneManager->getScreenCameraSetting() : m_SceneManager->getCameraSetting();

                    setting.defaultPosition = m_Camera->getPosition();
                    setting.defaultRotation = m_Camera->getRotation();
                    setting.defaultZoom     = m_Camera->getZoom();
                }
                else
                {
                    updateLog("loading camera state");

                    CameraSetting& setting = m_FrontScreen ? m_SceneManager->getScreenCameraSetting() : m_SceneManager->getCameraSetting();

                    m_Camera->setPosition(setting.defaultPosition);
                    m_Camera->setRotation(setting.defaultRotation);
                    m_Camera->setZoom(setting.defaultZoom);
                }
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
    void DevEngineUI::updateLog(const std::string& message, int level)
    {
        std::string _level;
        switch(level)
        {
            case nero::Info: {_level = "[Info]";}break;
            case nero::Debug: {_level = "[Debug]";}break;
            case nero::Warning: {_level = "[Warning]";}break;
        }

        m_LogLabel->SetText(m_LogLabel->GetText() + _level + " " +  message + "\n");
        m_LogAdjustment->SetValue(m_LogAdjustment->GetUpper());

        nero_log(message, level);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateLogIf(const std::string& message, bool condition, int level)
    {
        if(condition)
        {
            std::string _level;
            switch(level)
            {
                case nero::Info: {_level = "[Info]";}break;
                case nero::Debug: {_level = "[Debug]";}break;
                case nero::Warning: {_level = "[Warning]";}break;
            }

            m_LogLabel->SetText(m_LogLabel->GetText() + _level + " " + " " +  message + "\n");
            m_LogAdjustment->SetValue(m_LogAdjustment->GetUpper());

            nero_log_if(message, condition, level);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setSceneManager(SceneManager::Ptr sceneManager)
    {
       m_SceneManager = sceneManager;
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
        updateLog("## -- current Scene [" + m_SceneComboBox->GetSelectedText() + "] -- ## \n");

        //Stop Music
        if(m_ActiveSoundManager)
        {
            onStopMusic();
        }

        //Select the Scene
        m_SceneManager->setScene(m_SceneComboBox->GetSelectedText());

        //Update the active Managers
        m_ActiveSceneBuilder          = m_SceneManager->getSceneBuilder();
        m_ActiveMeshEditor            = m_SceneManager->getMeshEditor();
        m_ActiveUndoManager           = m_SceneManager->getUndoManager();
        m_ActiveSoundManager          = m_SceneManager->getSoundManager();
        m_ActiveGrid                  = m_SceneManager->getWorldGrid();

        //Canvas Color
        m_CanvasColor = m_ActiveSceneBuilder->getCanvasColor();
        //Update the Camera
        updateCamera(m_SceneManager->getCameraSetting());
        //Update the Scene Setting
        updateSceneSetting();
        //Update the Layer Table
        updateLayerTable();
        //Update Grid Setting
        updateGridSetting();
        //Update Sound Volume
        updateSoundSetting();
        //Update Screen
        updateSceneScreen();
        //
        m_EngineSetting.lastScene = m_SceneComboBox->GetSelectedText();
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
        if(!m_ActiveSceneBuilder) return;

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

            }break;

            case MESH:
            {
                m_EngineSubMode = MESH;
                m_ActiveSceneBuilder->updateAllLayerAlpha(MESH_MODE_ALPHA);
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
    void DevEngineUI::onFrontScreenToggleButton()
    {
        if(m_EngineMode == SCENE && (m_EngineSubMode == PLAY) || (m_EngineSubMode == MESH))
        {
            m_FrontScreenToggleButton->SetActive(false);
            return;
        }

        m_FrontScreen = m_FrontScreenToggleButton->IsActive();

        if(m_FrontScreen)
        {
            updateLog("changing View Mode to Screen View");

            //Desactivate submode button
            m_ObjectModeRadioButton->SetState(sfg::Widget::State::INSENSITIVE);
            m_MeshModeRadioButton->SetState(sfg::Widget::State::INSENSITIVE);
            m_PlayModeRadioButton->SetState(sfg::Widget::State::INSENSITIVE);

            //Update the Active Managers
            m_ActiveSceneBuilder          = m_SceneManager->getScreenBuilder();
            m_ActiveUndoManager           = m_SceneManager->getScreenUndoManager();
            m_ActiveGrid                  = m_SceneManager->getScreenGrid();
            m_ActiveMeshEditor            = nullptr;

            //Canvas Color
            m_CanvasColor = m_ActiveSceneBuilder->getCanvasColor();
            //Update the Camera
            updateCamera(m_SceneManager->getScreenCameraSetting());
            //Update the Layer Table
            updateLayerTable();
            //Update Grid Setting
            updateGridSetting();
            //
            m_CurrentView = "Screen View";
        }
        else
        {
            updateLog("changing View Mode to World View");

            //Activate submode button
            m_ObjectModeRadioButton->SetState(sfg::Widget::State::NORMAL);
            m_MeshModeRadioButton->SetState(sfg::Widget::State::NORMAL);
            m_PlayModeRadioButton->SetState(sfg::Widget::State::NORMAL);

            //Update the Active Managers
            m_ActiveSceneBuilder          = m_SceneManager->getSceneBuilder();
            m_ActiveMeshEditor            = m_SceneManager->getMeshEditor();
            m_ActiveUndoManager           = m_SceneManager->getUndoManager();
            m_ActiveGrid                  = m_SceneManager->getWorldGrid();

            //Canvas Color
            m_CanvasColor = m_ActiveSceneBuilder->getCanvasColor();
            //Update the Camera
            updateCamera(m_SceneManager->getCameraSetting());
            //Update the Layer Table
            updateLayerTable();
            //Update Grid Setting
            updateGridSetting();
            //
            m_CurrentView = "World View";
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onPlayButton()
    {
        if(!m_FrontScreen && m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            try
            {
                updateLog("playing [" + m_SceneComboBox->GetSelectedText() + "]");

                m_SceneManager->buildScene();
                setEngineSubMode(PLAY);
            }
            catch(std::exception const& e)
            {
                updateLog(e.what());
            }

        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onRenderButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
            updateLog("launching Scene Renderer with Scene [" + m_SceneComboBox->GetSelectedText() + "]");

            //launch the render
            m_RendererFuture = std::async(std::launch::async, [this]()
            {
                //create the scene
                Scene::Ptr scene = m_SceneManager->getScene();
                //create the renderer
                sf::Vector2f resolution = m_SceneManager->getSceneResolution(scene);
                std::string  name       = m_SceneComboBox->GetSelectedText();
                m_SceneRenderer         = SceneRenderer::Ptr(new SceneRenderer(resolution.x, resolution.y, name));

                //scene the scene to the renderer
                try
                {
                    m_SceneRenderer->setScene(scene);
                }
                catch(std::exception const& e)
                {
                    updateLog(e.what());
                    closeRenderer();
                }

                //m_SceneRenderer->setScene(scene);

                m_SceneRenderer->setRestartScene([this]()
                {
                    m_SceneRenderer->setScene(m_SceneManager->getScene());
                });

                m_SceneRenderer->setQuit([this]()
                {
                    closeRenderer();
                });

                m_SceneRenderer->run();

                m_SceneRenderer = nullptr;

                return 0;
            });
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::closeRenderer()
    {
        updateLog("closing Scene Renderer for Scene [" + m_SceneComboBox->GetSelectedText() + "]");

        m_RendererActive = false;
        m_SceneRenderer->m_Window.close();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onPauseButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            updateLog("pausing Scene [" + m_SceneComboBox->GetSelectedText() + "]");

            m_SceneManager->getSceneSetting().pause = !m_SceneManager->getSceneSetting().pause;
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onStepButton()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == PLAY)
        {
            updateLog("stepping Scene [" + m_SceneComboBox->GetSelectedText() + "]");
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
        updateLog("closing the Engine");

        //Stop Music
        if(m_ActiveSoundManager)
        {
            onStopMusic();
        }

        //Save Engine Setting
        updateLog("saving Engine Settings");
        saveFile(CONFIGURATION_FOLDER + "/" + ENGINE_CONFIGURATION + ".json", m_EngineSetting.toJson().dump(3));
        //Save all Scene
        updateLog("saving all Scenes");
        m_SceneManager->saveAllScene();
        //Quit
        m_Window.close();

        updateLog("\n ## -- Engine closed at " + getdate() + " " + getTime() + "-- ###\n");
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onUndoButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode == PLAY)
            return;

        nlohmann::json undo = m_ActiveUndoManager->undo();

        if(!undo.empty())
        {
            m_ActiveSceneBuilder->loadScene(undo);
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
            m_ActiveSceneBuilder->loadScene(redo);
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
        std::string scene_name  = m_SceneComboBox->GetSelectedText();
        std::string file        = WORKSPACE_FOLDER + "/" +  scene_name + "/" + scene_name +  ".json";
        updateLog("saving Scene [" + scene_name + "] to " + file, nero::Info);

        m_SceneManager->updateSceneSaveFile();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onLoadButton()
    {
        //Read and build scene from save file
        m_SceneManager->loadSceneSaveFile();
        //Update Layer
        updateLayerTable();
        //Update Undo
        updateUndo();
        //Update Scene Setting
        updateSceneSetting();
        //Update Grid
        updateGridSetting();
        //Update Sound Setting
        updateSoundSetting();
        //Update Scene Screens
        updateSceneScreen();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::buildCenter(sfg::Box::Ptr main_box)
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
        m_RenderButton              = sfg::Button::Create("Render");
        m_QuitButton                = sfg::Button::Create("Quit");
        m_UndoButton                = sfg::Button::Create("<--");
        m_RedoButton                = sfg::Button::Create("-->");

        black_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){updateLog("changing canvas color to black"); m_CanvasColor = sf::Color::Black; onColorRadioButton(); saveCanvasColor(m_CanvasColor);});
        gray_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){updateLog("changing canvas color to grey"); m_CanvasColor = sf::Color(119, 119, 119); onColorRadioButton(); saveCanvasColor(m_CanvasColor);});
        blue_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){updateLog("changing canvas color to blue"); m_CanvasColor = sf::Color(130, 177, 205); onColorRadioButton(); saveCanvasColor(m_CanvasColor);});
        white_color_button->GetSignal(sfg::Button::OnLeftClick).Connect([this](){updateLog("changing canvas color to white"); m_CanvasColor = sf::Color::White; onColorRadioButton(); saveCanvasColor(m_CanvasColor);});

        m_PlayButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onPlayButton, this));
        m_PauseButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onPauseButton, this));
        m_StepButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onStepButton, this));
        m_ResetButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onResetButton, this));
        m_RenderButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect([this](){if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT){m_RenderScene = true;}});
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
        utility_button_box_2->Pack(m_RenderButton);
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

        float base_width = win_width - 1305.f * 0.14f - 412.f;

        //Render_Canvas
        m_RenderCanvas              = sfg::Canvas::Create();
        m_CanvasFrontView           = m_RenderCanvas->GetView();
        //float frontViewWidth      = win_width*0.15f*3.63f-11.f;
        float frontViewWidth        = base_width - 11.f;
        float frontViewHeight       = win_height*0.75f-11.f;
        m_CanvasFrontView.setCenter(frontViewWidth/2.f, frontViewHeight/2.f);
        m_CanvasFrontView.setSize(sf::Vector2f(frontViewWidth, frontViewHeight));

        auto render_canvas_window   = sfg::Window::Create();
        render_canvas_window->SetStyle(sfg::Window::Style::BACKGROUND);
        render_canvas_window->SetRequisition(sf::Vector2f(base_width, win_height*0.75f));
        render_canvas_window->Add(m_RenderCanvas);

        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseEnter).Connect([this](){m_MouseOnCanvas = true; m_RenderCanvas->GrabFocus();});
        m_RenderCanvas->GetSignal(sfg::Canvas::OnMouseLeave).Connect([this](){m_MouseOnCanvas = false; m_Camera->cancelAction();});

        //Log_Window
        m_LogLabel = sfg::Label::Create();
        auto log_window_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
        auto log_window = sfg::ScrolledWindow::Create();
        log_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_NEVER);
        log_window->AddWithViewport(log_window_box);
        log_window->SetRequisition(sf::Vector2f(base_width, win_height*0.15f+4.f));
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
    void DevEngineUI::buildLeft(sfg::Box::Ptr main_box)
    {
        //Left : Top_Notebook | Bottom_Notebook

        float win_width = m_Window.getSize().x;
        float win_height = m_Window.getSize().y;

        //Top_Notebook : Utility | Music
        //Utility
        auto utility_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_utility_box(utility_view_box);

        //FrontScreen
        auto front_screen_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        build_front_screen_box(front_screen_view_box);

        //Music
        auto music_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        build_music_box(music_view_box);
        //Grid
        auto grid_view_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_grid_box(grid_view_box);

        auto top_notebook = sfg::Notebook::Create();
        top_notebook->AppendPage(utility_view_box, sfg::Label::Create("Utility"));
        top_notebook->AppendPage(front_screen_view_box, sfg::Label::Create("Screen"));
        top_notebook->AppendPage(music_view_box, sfg::Label::Create("Music"));
        top_notebook->AppendPage(grid_view_box, sfg::Label::Create("Grid"));

        //Bottom_Notebook : Sprite | Animation | Color | Mesh
        //Sprite
        auto sprite_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        auto sprite_window = sfg::ScrolledWindow::Create();
        sprite_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
        sprite_window->AddWithViewport(sprite_box);
        //sprite_window->SetRequisition(sf::Vector2f(win_width*0.14f, win_height*0.53f));
        sprite_window->SetRequisition(sf::Vector2f(1305.f*0.14f, win_height - 670.f*0.47f));
        build_sprite_box(sprite_box);
        sprite_box->GetSignal(sfg::Button::OnMouseLeave).Connect(std::bind(&DevEngineUI::onClosePreview, this));
        //Animation
        auto animation_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        auto animation_window = sfg::ScrolledWindow::Create();
        animation_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
        animation_window->AddWithViewport(animation_box);
        //animation_window->SetRequisition(sf::Vector2f(win_width*0.14f, win_height*0.53f));
        animation_window->SetRequisition(sf::Vector2f(1305.f*0.14f, win_height - 670.f*0.47f));
        build_animation_box(animation_box);
        animation_box->GetSignal(sfg::Button::OnMouseLeave).Connect(std::bind(&DevEngineUI::onClosePreview, this));

        //Color
        auto color_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        build_color_box(color_box);
        //Mesh
        auto mesh_window = sfg::Window::Create();
        mesh_window->SetStyle(sfg::Window::Style::BACKGROUND);
        //mesh_window->SetRequisition(sf::Vector2f(win_width*0.14f, 0.f));
        mesh_window->SetRequisition(sf::Vector2f(1305.f*0.14f, 0.f));
        build_mesh_window(mesh_window);
        //Sequence
        m_SequenceBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        m_SequenceNoteBook = sfg::Notebook::Create();
        m_SequenceNoteBook->SetId("layer_note_book");
        m_SequenceNoteBook->SetTabPosition(sfg::Notebook::TabPosition::BOTTOM);
        m_SequenceNoteBook->AppendPage(m_SequenceBox, sfg::Label::Create(""));
        //Text
        auto text_window = sfg::Window::Create();
        text_window->SetStyle(sfg::Window::Style::BACKGROUND);
        //text_window->SetRequisition(sf::Vector2f(win_width*0.14f, 0.f));
        text_window->SetRequisition(sf::Vector2f(1305.f*0.14f, 0.f));
        build_text_window(text_window);
        //Info
        auto info_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.f);
        build_info_box(info_box);

        //sequence window
        auto sequence_window = sfg::ScrolledWindow::Create();
        sequence_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC);
        sequence_window->AddWithViewport(m_SequenceNoteBook);
        //sequence_window->SetRequisition(sf::Vector2f(win_width*0.14f, win_height*0.53f));
        sequence_window->SetRequisition(sf::Vector2f(1305.f*0.14f, win_height - 670.f*0.47f));

        //
        m_ColorNotebook = sfg::Notebook::Create();
        m_ColorNotebook->AppendPage(sprite_window, sfg::Label::Create("Sprite"));
        m_ColorNotebook->AppendPage(animation_window, sfg::Label::Create("Animation"));
        m_ColorNotebook->AppendPage(sequence_window, sfg::Label::Create("Sequence"));
        m_ColorNotebook->AppendPage(mesh_window, sfg::Label::Create("Mesh"));
        m_ColorNotebook->AppendPage(text_window, sfg::Label::Create("Text"));
        m_ColorNotebook->AppendPage(color_box, sfg::Label::Create("Color"));
        m_ColorNotebook->AppendPage(info_box, sfg::Label::Create("Info"));

        m_ColorNotebook->SetScrollable(true);
        //m_ColorNotebook->SetRequisition(sf::Vector2f(win_width*0.160f, 0.f));
        m_ColorNotebook->SetRequisition(sf::Vector2f(1305.f*0.160f, 0.f));
        //m_ColorNotebook->GetSignal(sfg::Notebook::OnTabChange).Connect(std::bind(&DevEngineUI::onColorNotebook, this));

        //Left_End
        auto left_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        left_box->Pack(top_notebook);
        left_box->Pack(m_ColorNotebook);

        main_box->Pack(left_box);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_utility_box(sfg::Box::Ptr main_box)
    {
        //
        m_ObjectModeRadioButton     = sfg::RadioButton::Create("Object");
        m_MeshModeRadioButton       = sfg::RadioButton::Create("Mesh",  m_ObjectModeRadioButton->GetGroup());
        m_PlayModeRadioButton       = sfg::RadioButton::Create("Play",  m_ObjectModeRadioButton->GetGroup());


        m_SaveButton                = sfg::Button::Create("Save");
        m_LoadButton                = sfg::Button::Create("Load");
        m_AutoSaveChekButton        = sfg::CheckButton::Create("Auto save");

        m_AccessLearningButton      = sfg::Button::Create("Learn    ");
        m_AccessSnippetButton       = sfg::Button::Create("Snippet   ");
        m_AccessAPIButton           = sfg::Button::Create("Engine API");
        m_AccessForumButton         = sfg::Button::Create("Forum     ");

        auto box_1 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
        box_1->Pack(m_AccessLearningButton);
        box_1->Pack(m_AccessForumButton);
        auto box_2 = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);
        box_2->Pack(m_AccessSnippetButton);
        box_2->Pack(m_AccessAPIButton);
        auto box_3 = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        box_3->Pack(box_1, false);
        box_3->Pack(box_2, false);
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
        main_box->Pack(sub_mode_frame, false);
        main_box->Pack(save_frame, false);
        main_box->Pack(box_3);
        //

        m_ObjectModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_MeshModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_PlayModeRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onEngineSubModeButton, this));
        m_SaveButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onSaveButton, this));
        m_LoadButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onLoadButton, this));
        m_AutoSaveChekButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect([this]()
        {
            m_AutoSaveChekButton->IsActive() ? updateLog("enabling Auto Save") : updateLog("disabling Auto Save");

            m_EngineSetting.autoSave = m_AutoSaveChekButton->IsActive();
        });

        m_AccessLearningButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAccessLearningButton, this));
        m_AccessForumButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAccessForumButton, this));
        m_AccessSnippetButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAccessSnippetButton, this));
        m_AccessAPIButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAccessAPIButton, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_front_screen_box(sfg::Box::Ptr main_box)
    {
        m_ScreenComboBoxSecond       = sfg::ComboBox::Create();

        m_FrontScreenNameEntry = sfg::Entry::Create("");
        m_AddFrontScreenButton = sfg::Button::Create("Add");
        m_CopyFrontScreenButton = sfg::Button::Create("Copy");
        m_DeleteFrontScreenButton = sfg::Button::Create("Delete");
        m_RenameFrontScreenButton = sfg::Button::Create("Rename");

        auto frame = sfg::Frame::Create("Screen");
        auto frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 7.f);
        frame_box->Pack(m_ScreenComboBoxSecond, false);
        frame_box->Pack(m_FrontScreenNameEntry, false);
        frame->Add(frame_box);

        main_box->Pack(frame, false);
        main_box->Pack(m_AddFrontScreenButton, false);
        main_box->Pack(m_DeleteFrontScreenButton, false);
        main_box->Pack(m_RenameFrontScreenButton, false);

        m_ScreenComboBoxSecond->GetSignal(sfg::ComboBox::OnSelect).Connect([this](){m_FrontScreenNameEntry->SetText(m_ScreenComboBoxSecond->GetSelectedText());});
        m_AddFrontScreenButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAddFrontScreen, this));
        m_DeleteFrontScreenButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onDeleteFrontScreen, this));
        m_RenameFrontScreenButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onRenameFrontScreen, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_music_box(sfg::Box::Ptr music_box)
    {
        //Play table
        m_MusicComboBox = sfg::ComboBox::Create();
        m_SoundComboBox = sfg::ComboBox::Create();

        //Volume
        m_MusicAdjustment    = sfg::Adjustment::Create(0.f, 0.f, 100.f);
        m_SoundAdjustment    = sfg::Adjustment::Create(0.f, 0.f, 100.f);

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

        auto music_frame = sfg::Frame::Create("Play Music");
        music_frame->Add(m_MusicComboBox);

        auto sound_frame = sfg::Frame::Create("Play Sound");
        sound_frame->Add(m_SoundComboBox);

        auto volume_frame = sfg::Frame::Create("Volume");
        volume_frame->Add(volume_table);

        auto listener_frame = sfg::Frame::Create("Listener");
        listener_frame->Add(listener_table);

        auto stopMusicButton = sfg::Button::Create("Stop Music");

        music_box->Pack(music_frame, false);
        music_box->Pack(sound_frame, false);
        music_box->Pack(volume_frame, false);
        music_box->Pack(stopMusicButton, false);

        m_MusicComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onPlayMusic, this));
        m_SoundComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onPlaySound, this));
        stopMusicButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onStopMusic, this));
        m_MusicAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onMusicAdjustment, this));
        m_SoundAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onSoundAdjustment, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_sprite_box(sfg::Box::Ptr sprite_box)
    {
        auto spriteTable = m_ResourceManager->texture.getSpriteTable();
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
        auto animationTable    = m_ResourceManager->animation.getAnimationTable();
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
    void DevEngineUI::build_color_box(sfg::Box::Ptr color_box)
    {
        m_SpriteColorRadioButton        = sfg::RadioButton::Create("Sprite");
        m_LayerColorRadioButton         = sfg::RadioButton::Create("Layer");
        m_CanvasColorRadioButton        = sfg::RadioButton::Create("Canvas");
        m_GridColorRadioButton          = sfg::RadioButton::Create("Grid");
        m_TextColorRadioButton          = sfg::RadioButton::Create("Text");
        m_TextOutlineColorRadioButton   = sfg::RadioButton::Create("Text Outline");


        m_SpriteColorRadioButton->SetActive(true);
        m_LayerColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());
        m_CanvasColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());
        m_GridColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());
        m_TextColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());
        m_TextOutlineColorRadioButton->SetGroup(m_SpriteColorRadioButton->GetGroup());

        m_AlphaAdjustment   = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_RedAdjustment     = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_GreenAdjustment   = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);
        m_BlueAdjustment    = sfg::Adjustment::Create(255.f, 0.f, 255.f, 1.f, 5.f);

        auto table = sfg::Table::Create();
        table->Attach(m_SpriteColorRadioButton, sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_LayerColorRadioButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_CanvasColorRadioButton, sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_GridColorRadioButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_TextColorRadioButton, sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_TextOutlineColorRadioButton, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto frame = sfg::Frame::Create("Choose");

        frame->Add(table);

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

        color_box->Pack(frame, false);
        color_box->Pack(slider_frame, false);
        color_box->Pack(spinner_frame, false);

        m_AlphaAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_RedAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_GreenAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));
        m_BlueAdjustment->GetSignal(sfg::Adjustment::OnChange).Connect(std::bind(&DevEngineUI::onColorAdjustment, this));

        m_SpriteColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_LayerColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_CanvasColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_GridColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_TextColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
        m_TextOutlineColorRadioButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onColorRadioButton, this));
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

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_text_window(sfg::Window::Ptr text_window)
    {
        m_FontComboBox = sfg::ComboBox::Create();
        auto fontTable = m_ResourceManager->font.getFontTable();
        for(std::string font : fontTable)
        {
            m_FontComboBox->AppendItem(font);
        }

        m_TextEntry  = sfg::Entry::Create();

        m_AddTextButton = sfg::Button::Create("Add Text");

        m_TextFontSize          = sfg::SpinButton::Create(1.f, 100.f, 1.f);
        m_TextLetterSpacing     = sfg::SpinButton::Create(1.f, 100.f, 1.f);
        m_TextLineSpacing       = sfg::SpinButton::Create(1.f, 100.f, 1.f);
        m_TextOutlineThickness  = sfg::SpinButton::Create(1.f, 100.f, 1.f);

        m_TextBoldCheckButton  = sfg::CheckButton::Create("Bold");
        m_TextItalicCheckButton  = sfg::CheckButton::Create("Italic");
        m_TextUnderlinedCheckButton  = sfg::CheckButton::Create("Underline");
        m_TextStrikeThroughCheckButton  = sfg::CheckButton::Create("Line Through");

        m_TextFontSize->SetRequisition(sf::Vector2f(70.f, 0.f));

        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Font Size     "), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_TextFontSize, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Outline        "), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_TextOutlineThickness, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        //table->Attach(sfg::Label::Create("Letter Space"), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        //table->Attach(m_TextLetterSpacing, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        //table->Attach(sfg::Label::Create("Line Space  "), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        //table->Attach(m_TextLineSpacing, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto frame = sfg::Frame::Create("Style");
        auto frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.f);
        frame_box->Pack(m_TextBoldCheckButton);
        frame_box->Pack(m_TextItalicCheckButton);
        frame_box->Pack(m_TextUnderlinedCheckButton);
        frame_box->Pack(m_TextStrikeThroughCheckButton);
        frame->Add(frame_box);

        auto text_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 7.f);
        text_box->Pack(m_AddTextButton, false);
        text_box->Pack(m_FontComboBox, false);
        text_box->Pack(m_TextEntry, false);
        text_box->Pack(table, false);
        text_box->Pack(frame, false);

        text_window->Add(text_box);

        m_AddTextButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onAddTextButton, this));
        m_FontComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onFontComboBox, this));
        m_TextEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onTextEnty, this));
        m_TextFontSize->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onTextFontSize, this));
        m_TextLetterSpacing->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onTextLetterSpacing, this));
        m_TextLineSpacing->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onTextLineSpacing, this));
        m_TextOutlineThickness->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onTextOutlineThickness, this));
        m_TextBoldCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onTextStyle, this));
        m_TextItalicCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onTextStyle, this));
        m_TextUnderlinedCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onTextStyle, this));
        m_TextStrikeThroughCheckButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onTextStyle, this));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_info_box(sfg::Box::Ptr info_box)
    {
        m_ObjectPostionX = sfg::Label::Create("Position x");
        m_ObjectPostionY = sfg::Label::Create("Position y");
        m_ObjectRotation = sfg::Label::Create("Rotation");
        m_ObjectScale = sfg::Label::Create("Scale");

        m_ObjectPostionX->SetRequisition(sf::Vector2f(170.f, 0.f));
        m_ObjectPostionY->SetRequisition(sf::Vector2f(170.f, 0.f));
        m_ObjectRotation->SetRequisition(sf::Vector2f(170.f, 0.f));
        m_ObjectScale->SetRequisition(sf::Vector2f(170.f, 0.f));

        auto transform_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 4.f);
        transform_box->Pack(m_ObjectPostionX, false);
        transform_box->Pack(m_ObjectPostionY, false);
        transform_box->Pack(m_ObjectRotation, false);
        transform_box->Pack(m_ObjectScale, false);

        auto transform_frame = sfg::Frame::Create("Transform");
        transform_frame->Add(transform_box);

        info_box->Pack(transform_frame, false);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAddTextButton()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        sf::Vector2f screen_pos     = sf::Vector2f(m_RenderCanvas->GetAbsolutePosition().x + m_RenderCanvas->GetAllocation().width/2.f, m_RenderCanvas->GetAllocation().top + 150.f);
        sf::Vector2f world_pos      = canvas_to_world(screen_pos, m_RenderCanvas);

        /*if(CTRL() && !m_FrontScreen)
        {
            if(m_ActiveSceneBuilder->addObject(Object::Text_Meshed_Object, label, world_pos))
                updateLayerTable();
        }
        else if(SHIFT() && m_FrontScreen)
        {
            if(m_ActiveSceneBuilder->addObject(Object::Text_Button_Object, label, world_pos))
                updateLayerTable();
        }*/

        if(!CTRL_SHIFT_ALT())
        {
            if(m_ActiveSceneBuilder->addObject(Object::Text_Object, "New Text", world_pos))
                updateLayerTable();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onFontComboBox()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextFont(m_FontComboBox->GetSelectedText());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextEnty()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        std::string text = m_TextEntry->GetText();
        std::replace(text.begin(), text.end(), m_EngineSetting.carriageReturn.at(0), '\n');

        m_ActiveSceneBuilder->setTextContent(text);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextFontSize()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextFontSize(m_TextFontSize->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextLetterSpacing()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextLetterSpacing(m_TextLetterSpacing->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextLineSpacing()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextLineSpacing(m_TextLineSpacing->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextOutlineThickness()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextOutlineThickness(m_TextOutlineThickness->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onTextStyle()
    {
        if(m_EngineMode != SCENE || m_EngineSubMode != OBJECT)
                return;

        m_ActiveSceneBuilder->setTextStyle(
                    m_TextBoldCheckButton->IsActive(),
                    m_TextItalicCheckButton->IsActive(),
                    m_TextUnderlinedCheckButton->IsActive(),
                    m_TextStrikeThroughCheckButton->IsActive());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::buildRight(sfg::Box::Ptr main_box)
    {
        //Screen
        m_FrontScreenToggleButton   = sfg::ToggleButton::Create("Screen View");
        m_ScreenComboBoxFirst       = sfg::ComboBox::Create();
        m_FrontScreenToggleButton->GetSignal(sfg::ToggleButton::OnToggle).Connect(std::bind(&DevEngineUI::onFrontScreenToggleButton, this));
        m_ScreenComboBoxFirst->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onSelectFrontScreen, this));
        auto screen_frame = sfg::Frame::Create("Screen");
        auto screen_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        screen_box->Pack(m_FrontScreenToggleButton);
        screen_box->Pack(m_ScreenComboBoxFirst);
        screen_frame->Add(screen_box);

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
        left_box->Pack(screen_frame, false);
        left_box->Pack(mesh_box, false);
        left_box->Pack(object_box, false);
        left_box->Pack(layer_box, false);

        //Scene_Setting_Box
        auto right_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 2.f);
        build_scene_box(right_box);

        //Scene notebook box
        auto scene_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 5.f);
        scene_box->Pack(left_box);
        scene_box->Pack(right_box);

         //Help
        auto help_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 10.f);
        auto help_window = sfg::ScrolledWindow::Create();
        help_window->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS  | sfg::ScrolledWindow::HORIZONTAL_NEVER);
        help_window->AddWithViewport(help_box);
        help_box->SetRequisition(sf::Vector2f(350.f, 0.f));
        help_window->SetId("help_window");
        build_help_box(help_box);

        //Right_End
        m_EngineModeNotebook = sfg::Notebook::Create();
        m_EngineModeNotebook->AppendPage(scene_box, sfg::Label::Create("Scene"));
        m_EngineModeNotebook->AppendPage(help_window, sfg::Label::Create("Engine Help"));
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

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_object_box(sfg::Box::Ptr main_box)
    {
        m_ObjectNameEntry           = sfg::Entry::Create();
        m_ObjectCategoryEntry       = sfg::Entry::Create();
        m_ObjectMoveUpButton        = sfg::Button::Create("Move Up");
        m_ObjectMoveDownButton      = sfg::Button::Create("Move Down");

        m_ObjectNameEntry->SetRequisition(sf::Vector2f(100.f, 0.f));
        m_ObjectCategoryEntry->SetRequisition(sf::Vector2f(100.f, 0.f));

        m_ObjectNameEntry->SetId("little_font_size");
        m_ObjectCategoryEntry->SetId("little_font_size");

        auto table = sfg::Table::Create();
        table->Attach(sfg::Label::Create("Name    "), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_ObjectNameEntry, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(sfg::Label::Create("Category"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        table->Attach(m_ObjectCategoryEntry, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto button_box = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 0.f);
        button_box->Pack(m_ObjectMoveUpButton);
        button_box->Pack(m_ObjectMoveDownButton);

        auto frame_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        frame_box->Pack(table);
        frame_box->Pack(button_box);

        auto frame = sfg::Frame::Create("Object");
        frame->Add(frame_box);

        main_box->Pack(frame, false);

        m_ObjectNameEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onObjectNameEntry, this));
        m_ObjectCategoryEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(std::bind(&DevEngineUI::onObjectCategoryEntry, this));

        m_ObjectMoveUpButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onMoveObjectUp, this));
        m_ObjectMoveDownButton->GetSignal(sfg::Button::OnLeftClick).Connect(std::bind(&DevEngineUI::onMoveObjectDown, this));
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
        window->SetRequisition(sf::Vector2f(0.f, m_Window.getSize().y - 670.f*0.64f));

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
        auto layer_box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
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

        m_DrawAxisCheckButton               = sfg::CheckButton::Create("Info");
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

        m_SceneComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind(&DevEngineUI::onSceneSelection, this));

        m_xGravitySpinButton->GetSignal(sfg::SpinButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onScenePinButton, this));
        m_yGravitySpinButton->GetSignal(sfg::SpinButton::OnLeftClick).Connect(std::bind(&DevEngineUI::onScenePinButton, this));
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
        m_GridWidthSpinButton       =   sfg::SpinButton::Create(5.f, MAX_VALUE, 1.f);
        m_GridHeightSpinButton      =   sfg::SpinButton::Create(5.f, MAX_VALUE, 1.f);
        m_GridXSizeSpinButton       =   sfg::SpinButton::Create(1.f, MAX_VALUE, 1.f);
        m_GridYSizeSpinButton       =   sfg::SpinButton::Create(1.f, MAX_VALUE, 1.f);
        m_GridXOffsetSpinButton     =   sfg::SpinButton::Create(-MAX_VALUE, MAX_VALUE, 1.f);
        m_GridYOffsetSpinButton     =   sfg::SpinButton::Create(-MAX_VALUE, MAX_VALUE, 1.f);

        m_GridWidthSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_GridHeightSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_GridXSizeSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_GridYSizeSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_GridXOffsetSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));
        m_GridYOffsetSpinButton->SetRequisition(sf::Vector2f(80.f, 0.f));

        m_GridWidthSpinButton->SetId("grid_entry");
        m_GridHeightSpinButton->SetId("grid_entry");
        m_GridXSizeSpinButton->SetId("grid_entry");
        m_GridYSizeSpinButton->SetId("grid_entry");
        m_GridXOffsetSpinButton->SetId("grid_entry");
        m_GridYOffsetSpinButton->SetId("grid_entry");

        auto tile_table = sfg::Table::Create();
        tile_table->Attach(sfg::Label::Create("Width"), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        tile_table->Attach(m_GridWidthSpinButton, sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        tile_table->Attach(sfg::Label::Create("Height"), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        tile_table->Attach(m_GridHeightSpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto grid_table = sfg::Table::Create();
        grid_table->Attach(sfg::Label::Create("X-Count"), sf::Rect<sf::Uint32>( 0,1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(m_GridXSizeSpinButton, sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(sfg::Label::Create("Y-Count"), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(m_GridYSizeSpinButton, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        grid_table->Attach(sfg::Label::Create("X-Offset"), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(m_GridXOffsetSpinButton, sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(sfg::Label::Create("Y-Offset"), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );
        grid_table->Attach(m_GridYOffsetSpinButton, sf::Rect<sf::Uint32>( 1, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::EXPAND, sf::Vector2f( 2.f, 2.f ) );

        auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
        box->Pack(tile_table);
        box->Pack(sfg::Separator::Create());
        box->Pack(grid_table);

        auto frame = sfg::Frame::Create("Grid Cell");
        frame->Add(box);

        main_box->Pack(frame);

        m_GridWidthSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridXSizeChange, this));
        m_GridHeightSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridYSizeChange, this));
        m_GridXSizeSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridXCountChange, this));
        m_GridYSizeSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridYCountChange, this));
        m_GridXOffsetSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridXMove, this));
        m_GridYOffsetSpinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect(std::bind(&DevEngineUI::onGridYMove, this));
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

            if(CTRL() && !m_FrontScreen)
            {
                if(m_ActiveSceneBuilder->addObject(Object::Meshed_Object, label, world_pos))
                    updateLayerTable();
            }
            else if(SHIFT() && m_FrontScreen)
            {
                if(m_ActiveSceneBuilder->addObject(Object::Button_Object, label, world_pos))
                    updateLayerTable();
            }
            else if(!CTRL_SHIFT_ALT())
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
                m_PreviewTexture = m_ResourceManager->texture.getSpriteTexture(label);
                sf::IntRect bound = m_ResourceManager->texture.getSpriteBound(label);
                m_Preview.setTextureRect(bound);
                m_Preview.setTexture(m_PreviewTexture);
                m_Preview.setOrigin(bound.width/2.f, bound.height/2.f);
                m_Preview.setPosition(m_CanvasFrontView.getSize().x/2.f, bound.height/2.f + 100.f);
                m_Preview.setColor(sf::Color(255, 255, 255, m_EngineSetting.previewAlpha));
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

            if(CTRL() && !m_FrontScreen)
            {
                if(m_ActiveSceneBuilder->addObject(Object::Animation_Meshed_Object, label, world_pos))
                    updateLayerTable();
            }
            /*else if(SHIFT() && m_FrontScreen)
            {
                if(m_ActiveSceneBuilder->addObject(Object::Animation_Button_Object, label, world_pos))
                    updateLayerTable();
            }*/
            else if(!CTRL_SHIFT_ALT())
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
            m_PreviewTexture = m_ResourceManager->animation.getTexture(label);
            sf::IntRect bound = m_ResourceManager->animation.getAnimationBound(label);
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

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateUI()
    {
        Object::Ptr object = m_ActiveSceneBuilder->getSelectedObject();

        m_ObjectNameEntry->SetText(object->getName());
        m_ObjectCategoryEntry->SetText(object->getCategory());

        //Update Color Tab
        if((object->getFirstType() == Object::Sprite_Object || object->getFirstType() == Object::Animation_Object) && m_SpriteColorRadioButton->IsActive())
        {
            sf::Color c = object->getColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }

        //Update Mesh Tab
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

        //Update Animation Sequence Tab
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
                    updateLog("changing animation object [" + animation_object->getName() + "] sequence to [" + sequence + "]");
                    animation_object->setSequence(sequence);
                });

                spinButton->GetSignal(sfg::SpinButton::OnValueChanged).Connect([this, animation_object, spinButton, sequence]()
                {
                    updateLog("changing sequence [" + sequence + "] frame rate to [" + toString(1.f/spinButton->GetValue()) + "] in animation object [" + animation_object->getName() + "]");
                    animation_object->setSequence(sequence);
                    animation_object->setFrameRate(1.f/spinButton->GetValue());
                });

                checkButton->GetSignal(sfg::CheckButton::OnLeftClick).Connect([this, animation_object, checkButton, sequence]()
                {
                    std::string status = checkButton->IsActive() ? "enabling" : "disabling";
                    updateLog(status + " loop for sequence [" + sequence + "] in animation object [" + animation_object->getName() + "]");

                    animation_object->setSequence(sequence);
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

        //Update Text tab
        if(object->getFirstType() == Object::Text_Object)
        {
            TextObject::Ptr text_object = TextObject::Cast(object);

            for(int i = 0; i < m_FontComboBox->GetItemCount(); i++)
            {
                if(m_FontComboBox->GetItem(i) == text_object->getFont())
                {
                    m_FontComboBox->SelectItem(i);
                    break;
                }
            }

            std::string text =text_object->getContent();
            std::replace(text.begin(), text.end(), '\n', m_EngineSetting.carriageReturn.at(0));
            m_TextEntry->SetText(text);

            m_TextFontSize->SetValue(text_object->getFontSize());
            m_TextLetterSpacing->SetValue(text_object->getLetterSpacing());
            m_TextLineSpacing->SetValue(text_object->getLineSpacing());
            m_TextOutlineThickness->SetValue(text_object->getOutlineThickness());

            m_TextBoldCheckButton->SetActive(text_object->isBold());
            m_TextItalicCheckButton->SetActive(text_object->isItalic());
            m_TextUnderlinedCheckButton->SetActive(text_object->isUnderlined());
            m_TextStrikeThroughCheckButton->SetActive(text_object->isStrikeThrough());
        }

        //Update Info tab
        {
            m_ObjectPostionX->SetText("Position x : " + toString(object->getPosition().x));
            m_ObjectPostionY->SetText("Position y : " + toString(object->getPosition().y));
            m_ObjectRotation->SetText("Rotation : " + toString(object->getRotation()));
            m_ObjectScale->SetText("Scale : " + toString(object->getScale().x));
        }

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateUndo()
    {
        m_ActiveUndoManager->add(m_ActiveSceneBuilder->saveScene());
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
            case Object::Button_Object: layer.nameEntry->SetId("button_layer"); break;
            case Object::Text_Object: layer.nameEntry->SetId("text_layer"); break;
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

    ////////////////////////////////////////////////////////////
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
    void DevEngineUI::onMoveObjectUp()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
        {
             m_ActiveSceneBuilder->moveObjectUp();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMoveObjectDown()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT)
            m_ActiveSceneBuilder->moveObjectDown();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMeshComboBox()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT && !m_FrontScreen)
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
        {
            m_ActiveSceneBuilder->setMeshType(m_MeshTypeComboBox->GetSelectedText());
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onPlayMusic()
    {
        updateLog("playing music [" + toString(m_MusicComboBox->GetSelectedText()) + "]");
        m_ActiveSoundManager->playMusic(toString(m_MusicComboBox->GetSelectedText()));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onPlaySound()
    {
        updateLog("playing sound [" + toString(m_SoundComboBox->GetSelectedText()) + "]");
        m_ActiveSoundManager->playSound(toString(m_SoundComboBox->GetSelectedText()));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onStopMusic()
    {
        updateLog("stopping music");
        m_ActiveSoundManager->stopMusic(toString(m_MusicComboBox->GetSelectedText()));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onMusicAdjustment()
    {
        m_ActiveSoundManager->setMusicVolume(m_MusicAdjustment->GetValue());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onSoundAdjustment()
    {
        m_ActiveSoundManager->setSoundVolume(m_SoundAdjustment->GetValue());
    }

    ////////////////////////////////////////////////////////////
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
            sf::Color c = m_ActiveSceneBuilder->getLayerColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);

        }
        else if (m_CanvasColorRadioButton->IsActive())
        {
            sf::Color c = m_CanvasColor;

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }
        else if (m_GridColorRadioButton->IsActive())
        {
            sf::Color c = m_ActiveGrid->getColor();

            m_AlphaAdjustment->SetValue(c.a);
            m_RedAdjustment->SetValue(c.r);
            m_GreenAdjustment->SetValue(c.g);
            m_BlueAdjustment->SetValue(c.b);
        }
        else if(m_TextColorRadioButton->IsActive())
        {
            Object::Ptr object = m_ActiveSceneBuilder->getSelectedObject();

            if(object && object->getFirstType() == Object::Text_Object)
            {
                 sf::Color c = TextObject::Cast(object)->getColor();

                m_AlphaAdjustment->SetValue(c.a);
                m_RedAdjustment->SetValue(c.r);
                m_GreenAdjustment->SetValue(c.g);
                m_BlueAdjustment->SetValue(c.b);
            }
        }
        else if(m_TextOutlineColorRadioButton->IsActive())
        {
           Object::Ptr object = m_ActiveSceneBuilder->getSelectedObject();

            if(object && object->getFirstType() == Object::Text_Object)
            {
                sf::Color c = TextObject::Cast(object)->getOutlineColor();

                m_AlphaAdjustment->SetValue(c.a);
                m_RedAdjustment->SetValue(c.r);
                m_GreenAdjustment->SetValue(c.g);
                m_BlueAdjustment->SetValue(c.b);
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onColorAdjustment()
    {
        sf::Color color(m_RedAdjustment->GetValue(), m_GreenAdjustment->GetValue(), m_BlueAdjustment->GetValue(), m_AlphaAdjustment->GetValue());

        //update picker
        if(!IMGUI_COLOR_PICKER_OPEN)
        {
            m_ColorPickerColor.x = color.r/255.f;
            m_ColorPickerColor.y = color.g/255.f;
            m_ColorPickerColor.z = color.b/255.f;
            m_ColorPickerColor.w = color.a/255.f;
        }

        if(m_LayerColorRadioButton->IsActive())
        {
            m_ActiveSceneBuilder->updateLayerColor(color);
        }
        else if(m_SpriteColorRadioButton->IsActive())
        {
            m_ActiveSceneBuilder->updateSpriteColor(color);
        }
        else if (m_CanvasColorRadioButton->IsActive())
        {
            m_CanvasColor = color;
            saveCanvasColor(m_CanvasColor);
        }
        else if(m_GridColorRadioButton->IsActive())
        {
            m_ActiveGrid->setColor(color);
        }
        else if(m_TextColorRadioButton->IsActive())
        {
             m_ActiveSceneBuilder->updateTextColor(color);
        }
        else if(m_TextOutlineColorRadioButton->IsActive())
        {
            m_ActiveSceneBuilder->updateOutlineTextColor(color);
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::saveCanvasColor(const sf::Color& color)
    {
        m_ActiveSceneBuilder->setCanvasColor(color);
    }

    ////////////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateCameraSetting()
    {
        CameraSetting& setting = m_FrontScreen ? m_SceneManager->getScreenCameraSetting() : m_SceneManager->getCameraSetting();

        setting.position    = m_Camera->getPosition();
        setting.rotation    = m_Camera->getRotation();
        setting.zoom        = m_Camera->getZoom();
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::autoSave()
    {
        if(m_EngineMode == SCENE && m_EngineSubMode == OBJECT && m_EngineSetting.autoSave)
        {
            m_ElapseTime += m_AutoSaveClock.restart();
            if(m_ElapseTime > sf::seconds(m_EngineSetting.autoSaveTime))
            {
                m_ElapseTime -= sf::seconds(m_EngineSetting.autoSaveTime);
                m_SceneManager->updateSceneSaveFile();
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

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridXSizeChange()
    {
        //update the grid
        sf::Vector2i tileSize;
        tileSize.x  = static_cast<int>(m_GridWidthSpinButton->GetValue());
        tileSize.y = m_ActiveGrid->getTileSize().y;

        m_ActiveGrid->update(tileSize, m_ActiveGrid->getTileCount());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridYSizeChange()
    {
        //update the grid
        sf::Vector2i tileSize;
        tileSize.y  = static_cast<int>(m_GridHeightSpinButton->GetValue());
        tileSize.x = m_ActiveGrid->getTileSize().x;

        m_ActiveGrid->update(tileSize, m_ActiveGrid->getTileCount());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridXCountChange()
    {
        //update the grid
        sf::Vector2i tileCount;
        tileCount.x  = static_cast<int>(m_GridXSizeSpinButton->GetValue());
        tileCount.y = m_ActiveGrid->getTileCount().y;

        m_ActiveGrid->update(m_ActiveGrid->getTileSize(), tileCount);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridYCountChange()
    {
        //update the grid
        sf::Vector2i tileCount;
        tileCount.y  = static_cast<int>(m_GridYSizeSpinButton->GetValue());
        tileCount.x = m_ActiveGrid->getTileCount().x;

        m_ActiveGrid->update(m_ActiveGrid->getTileSize(), tileCount);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridXMove()
    {
        sf::Vector2i position;
        position.x  = static_cast<int>(m_GridXOffsetSpinButton->GetValue());
        position.y = m_ActiveGrid->getPosition().y;

        m_ActiveGrid->updatePosition(position);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onGridYMove()
    {
        sf::Vector2i position;
        position.y  = static_cast<int>(m_GridYOffsetSpinButton->GetValue());
        position.x = m_ActiveGrid->getPosition().x;

        m_ActiveGrid->updatePosition(position);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateGridSetting()
    {
        m_GridWidthSpinButton->SetValue(m_ActiveGrid->getTileSize().x);
        m_GridHeightSpinButton->SetValue(m_ActiveGrid->getTileSize().y);
        m_GridXSizeSpinButton->SetValue(m_ActiveGrid->getTileCount().x);
        m_GridYSizeSpinButton->SetValue(m_ActiveGrid->getTileCount().y);
        m_GridXOffsetSpinButton->SetValue(m_ActiveGrid->getPosition().x);
        m_GridYOffsetSpinButton->SetValue(m_ActiveGrid->getPosition().y);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateSoundSetting()
    {
        m_SoundAdjustment->SetValue(m_ActiveSoundManager->getSoundVolume());
        m_MusicAdjustment->SetValue(m_ActiveSoundManager->getMusicVolume());
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateCamera(const CameraSetting& cameraSetting)
    {
        m_Camera->setPosition(cameraSetting.position);
        m_Camera->setRotation(cameraSetting.rotation);
        m_Camera->setZoom(cameraSetting.zoom);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateSceneScreen()
    {
        m_ScreenComboBoxFirst->Clear();
        m_ScreenComboBoxSecond->Clear();

        //Add Scene Screens
        auto screenTable = m_SceneManager->getScreenTable();
        for(auto screen : screenTable)
        {
            m_ScreenComboBoxFirst->AppendItem(screen);
            m_ScreenComboBoxSecond->AppendItem(screen);
        }

        m_ScreenComboBoxFirst->SelectItem(0);
        m_ScreenComboBoxSecond->SelectItem(0);
    }


    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAddFrontScreen()
    {
        if(m_SceneManager->addScreen(m_FrontScreenNameEntry->GetText()))
        {
            m_FrontScreenNameEntry->SetText("");
            updateSceneScreen();
            onSaveButton();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onDeleteFrontScreen()
    {
        if(m_SceneManager->deleteScreen(m_ScreenComboBoxSecond->GetSelectedText()))
        {
            m_FrontScreenNameEntry->SetText("");
            updateSceneScreen();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onRenameFrontScreen()
    {
        if(m_SceneManager->renameScreen(m_ScreenComboBoxSecond->GetSelectedText(), m_FrontScreenNameEntry->GetText()))
        {
            m_FrontScreenNameEntry->SetText("");
            updateSceneScreen();
            onSaveButton();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onSelectFrontScreen()
    {
        updateLog("changing Screen to " + m_ScreenComboBoxFirst->GetSelectedText());

        m_SceneManager->selectScreen(m_ScreenComboBoxFirst->GetSelectedText());

        if(m_FrontScreen)
        {
            onFrontScreenToggleButton();
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAccessLearningButton()
    {
        #ifdef _WIN32
            updateLog("openning browser at : https://nero-games.com/learn");
            ShellExecute(NULL, "open", "https://nero-games.com/learn", NULL, NULL, SW_SHOWNORMAL);
        #else
            updateLog("openning browser at : https://nero-games.com/learn");
            system("https://nero-games.com/learn");
        #endif // _WIN32
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAccessForumButton()
    {
        #ifdef _WIN32
            updateLog("openning browser at : https://nero-games.com/forum/index.php");
            ShellExecute(NULL, "open", "https://nero-games.com/forum/index.php", NULL, NULL, SW_SHOWNORMAL);
        #else
            updateLog("openning browser at : https://nero-games.com/forum/index.php");
            system("https://nero-games.com/forum/index.php");
        #endif // _WIN32
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAccessSnippetButton()
    {
        #ifdef _WIN32
            updateLog("openning browser at : https://nero-games.com/snippet");
            ShellExecute(NULL, "open", "https://nero-games.com/snippet", NULL, NULL, SW_SHOWNORMAL);
        #else
            updateLog("openning browser at : https://nero-games.com/snippet");
            system("https://nero-games.com/snippet");
        #endif // _WIN32
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onAccessAPIButton()
    {
        #ifdef _WIN32
            updateLog("openning browser at : https://nero-games.com/learn/engine-v1/cpp-api/");
            ShellExecute(NULL, "open", "https://nero-games.com/learn/engine-v1/cpp-api/", NULL, NULL, SW_SHOWNORMAL);
        #else
            updateLog("openning browser at : https://nero-games.com/learn/engine-v1/cpp-api/");
            system("https://nero-games.com/learn/engine-v1/cpp-api/");
        #endif // _WIN32
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::buildCamera()
    {
        float camera_width = m_Window.getSize().x - 1305.f * 0.14f - 412.f;

        m_Camera = AdvancedCamera::Ptr(new AdvancedCamera(sf::Vector2f(camera_width, m_Window.getSize().y*0.75f), m_RenderCanvas));
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::buildSceneManger()
    {
        m_SceneManager = SceneManager::Ptr(new SceneManager(Scene::Context(m_RenderCanvas, m_CanvasFrontView, m_Camera, m_ResourceManager, false)));
        m_SceneManager->setUpdateUI(std::bind(&DevEngineUI::updateUI, this));
        m_SceneManager->setUpdateUndo(std::bind(&DevEngineUI::updateUndo, this));
        m_SceneManager->setUpdateLog(std::bind(&DevEngineUI::updateLog, this, std::placeholders::_1, std::placeholders::_2));
        m_SceneManager->setUpdateLogIf(std::bind(&DevEngineUI::updateLogIf, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::setResourceManager(ResourceManager::Ptr resourceManager)
    {
        m_ResourceManager = resourceManager;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateFrameRate(const float& frameRate, const float& frameTime)
    {
        m_FrameRate = frameRate;
        m_FrameTime = frameTime;
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::updateInfo()
    {
        m_InfoText.setString(m_CurrentView + "  |  " + toString(m_FrameRate) + " fps  |  " + toString(m_FrameTime * 1000.f) + " ms");
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::renderColorPicker()
    {
        ImGui::SFML::Update(m_Window, TIME_PER_FRAME);

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x/2.f -250.f, ImGui::GetIO().DisplaySize.y - 35.f), ImGuiCond_Always, ImVec2(0.f, 0.));
        //ImGui::SetNextWindowBgAlpha(0.f); // Transparent background
        if (ImGui::Begin("color_picker", (bool*)true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
        {
            ImGui::ColorEdit4("color", &m_ColorPickerColor.x, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop);
        }
        ImGui::End();

        ImGui::SFML::Render(m_Window);
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::onColorNotebook()
    {
        switch(m_ColorNotebook->GetCurrentPage())
        {
            case 5:
            {
                m_RenderColorPicker = true;
            }break;

            default:
            {
                m_RenderColorPicker = false;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    void DevEngineUI::build_help_box(sfg::Box::Ptr help_box)
    {
        int countHelp = 0;
        auto addHelp = [&help_box, &countHelp](const std::string& header, const std::string& content)
        {
            if(countHelp > 0)
            {
                help_box->Pack(sfg::Separator::Create());
            }
            auto help = sfg::Label::Create(header + "\n-------------------------------------------------------------------\n\n" + content + "\n");
            help->SetLineWrap(true);
            auto frame = sfg::Frame::Create("#-" + toString(countHelp++));
            frame->Add(help);
            help_box->Pack(frame);
        };

        addHelp("Engine Quick Help",
                "Welcome to the help section ! Here you can learn how to interact with the Engine, for more information please access the Engine Learning-Page using the button [ Learn ] on the left");

        addHelp("Access Buttons",
                std::string() + "On the Engine left you have four buttons : Learn - Forum - Snippet - Engine API. Those buttons let you access quickly to some sections of the Engine website"
                + "\n\n" + "- [Learn] Learn how to build games"
                + "\n\n" + "- [Forum] Ask any question here"
                + "\n\n" + "- [Snippet] Find some useful chunks of code"
                + "\n\n" + "- [Engine API] Access the Engine C++ Classes definition");

        addHelp("The Canvas",
                std::string("")+ "The Canvas is the big black area on the middle. The grid is visible by default on the canvas."
                + "you can change the canvas color as you want. Try clicking on the buttons [Black] [Grey] [Blue] [White] located on top of the Canvas in the left and right corner");

        addHelp("Log View",
                "Below the Canvas you can find the Log-View, From time to time it is good to get a look at the logs");

        addHelp("Mouse on Canvas",
                std::string("") + "Certain actions are possible only when the mouse is on the Canvas. Example :"
                + "\n\n" + "- Control the camera with the Numpad or the Mouse"
                + "\n\n" + "- Change the Engine Scene Mode with the SpaceBar"
                + "\n\n" + "- Play, Pause, Reset, Step a Scene via shortcuts");

        addHelp("Control Overview",
                std::string("") + "The Engine uses the following Keys"
                + "\n\n" + "- Modifier : CTRL, SHIFT, ALT"
                + "\n\n" + "- Mouse : Buttons (Left, Right, Middle), Wheel"
                + "\n\n" + "- Numpad : 1 to 9, Multiply, Divide"
                + "\n\n" + "- B, Y, Z, S, L"
                + "\n\n" + "- Arrows : Left, Up, Right"
                + "\n\n" + "- SpaceBar, Return, Delete, PageUp, PageDown");

        addHelp("The Camera",
                std::string("") + "The Camera can be controlled with the Numpad or the Mouse"
                + "\n\n" + "- Numpad_4 : move left"
                + "\n\n" + "- Numpad_6 : move right"
                + "\n\n" + "- Numpad_8 : move up"
                + "\n\n" + "- Numpad_2 : move down"
                + "\n\n" + "- Numpad_7 : rotate left"
                + "\n\n" + "- Numpad_9 : rotate right"
                + "\n\n" + "- Numpad_Multiply : zoom in"
                + "\n\n" + "- Numpad_Divide : zoom out"
                + "\n\n" + "- Numpad_5 : reset camera state"
                + "\n\n" + "- ALT + Numpad_5 : save camera state"
                + "\n\n" + "- CTRL + MouseWheel : move left or right"
                + "\n\n" + "- ALT + MouseWheel : move up or down"
                + "\n\n" + "- SHIFT + MouseWheel : rotate left/right"
                + "\n\n" + "- ALT + Mouse MiddleButton : save camera state"
                + "\n\n" + "- Mouse MiddleButton : reset camera state"
                + "\n\n" + "- MouseWheel : zoom in or out");

        addHelp("Select a Scene",
                std::string("") + "The Scene is main the component of the Engine. A Scene represents a Game. "
                + "On the right in the [Scene] Tab you have a selection box that allows to select a Scene you want to work on. "
                + "\n\n" +
                + "It is also possible to change the Scene with the keys PageUp and PageDown");

        addHelp("The Three(3) Scene Mode",
                std::string("") + "The Engine has three (3) Scene Mode : Object, Mesh, Play"
                + "\n\n" + "- Object Mode :"
                + "\n\n" + "Add, Modify, Duplicate, Delete Objects"
                + "\n\n" + "- Mesh Mode :"
                + "\n\n" + "Modify mesh (add vertices, extrude lines"
                + "\n\n" + "- Play Mode :"
                + "\n\n" + "Test your Scene in real time"
                + "\n\n"
                + "\n\n"
                + "The SpaceBar hollows to switch between the Modes"
                + "\n\n" + "- Press SpaceBar : switch between Object and Mesh modes"
                + "\n\n" + "- CTRL + SpaceBar : access Play mode");

        addHelp("Undo and Redo",
                std::string("") + "Most actions on Objects can be canceled on re-applied"
                + "\n\n" + "- Undo : Ctrl + Z"
                + "\n\n" + "- Redo : Ctrl + Y");

        addHelp("Save and Load Scene",
                std::string("") + "A Scene can be Saved or Loaded at any moment"
                + "\n\n" + "- Save : Ctrl + S"
                + "\n\n" + "- Load : Ctrl + L"
                + "\n\n"
                + "By default the Engine performs a Auto-Save each thirty (30) seconds");

        addHelp("World View and Screen View",
                std::string("") + "On the right, in the Tab [Scene] there is a button called [Screen View]. "
                + "This button allows to switch between the World View and the Screen View of your current Scene. "
                + "Not all types of Object can be added on each View mode."
                + "\n\n"
                + "- World View : "
                + "\n\n"
                + "Sprite_Object, Animation_Object, Mesh_Object, Meshed_Sprite_Object, Meshed_Animation_Object, Text_Object"
                + "\n\n"
                + "- Screen View : "
                + "\n\n"
                + "Sprite_Object, Animation_Object, Text_Object, Button_Object");

        addHelp("Add Objects on the Canvas",
                std::string("") + "The way you add an Object onto the Canvas differs with the type of Object. Also"
                + "Object can be added only on Object Mode"
                + "\n\n" +
                + "- Add Sprite_Object : click on a [Sprite_Button] in the [Sprite] Tab located on the left"
                + "\n\n" +
                + "- Add Animation_Object : click on a [Animation_Button] in the [Animation] Tab located on the left"
                + "\n\n" +
                + "- Add Meshed_Sprite_Object : hold CTRL and click on a [Sprite_Button]"
                + "\n\n" +
                + "- Add Meshed_Animation_Object : hold CTRL and click on a [Animation_Button]"
                + "\n\n" +
                + "- Add Mesh_Object : Select a Mesh [Polygon, Circle, Line] from the DropDown on the right"
                + "\n\n" +
                + "- Add Text_Object : click on the button [Add Text] in the [Text] Tab located on the left"
                + "\n\n" +
                + "- Add Button_Object : hold SHIFT and click on a [Sprite_Button]");

        addHelp("Remove Objects from the Canvas",
                std::string("") + "When an Object is selected it can be remove by two ways"
                + "\n\n" + "- ALT + Mouse Left Button"+
                + "\n\n" + "- Delete Button");

        addHelp("Play with Objects",
                std::string("") + "Objects can be selected with the Mouse Right Button."
                + "When an Object is selected there are various actions that can be on it."
                + "\n\n" + "- Select Object : Mouse Right click"
                + "\n\n" + "- Un-select Object : Mouse Left on empty space"
                + "\n\n"
                + "The following actions can be applied to any Object"
                + "\n\n" + "- CTRL + Numpad_4 : move left"
                + "\n\n" + "- CTRL + Numpad_6 : move right"
                + "\n\n" + "- CTRL + Numpad_8 : move up"
                + "\n\n" + "- CTRL + Numpad_2 : move down"
                + "\n\n" + "- CTRL + Numpad_7 : rotate left"
                + "\n\n" + "- CTRL + Numpad_9 : rotate right"
                + "\n\n" + "- CTRL + Numpad_Multiply : zoom in"
                + "\n\n" + "- CTRL + Numpad_Divide : zoom out"
                + "\n\n" + "- CTRL + Mouse Move Up or Down : zoom in or out"
                + "\n\n" + "- SHIFT + Mouse Move Up or Down : rotate left or right"
                + "\n\n" + "- ALT + Numpad_4 : copy left"
                + "\n\n" + "- ALT + Numpad_6 : copy right"
                + "\n\n" + "- ALT + Numpad_8 : copy up"
                + "\n\n" + "- ALT + Numpad_2 : copy down"
                + "\n\n" + "- ALT + Numpad_7 : copy up left"
                + "\n\n" + "- ALT + Numpad_9 : copy up right"
                + "\n\n" + "- ALT + Numpad_1 : copy down left"
                + "\n\n" + "- ALT + Numpad_3 : copy down right"
                + "\n\n"
                + "The following actions can be applied to a Text_Object or all Objects containing a Sprite that means:\n"
                + "Sprite_Object, Animation_Object, Meshed_Sprite_Object, Meshed_Animation_Object"
                + "\n\n" + "- CTRL + Numpad_1 : flip horizontally"
                + "\n\n" + "- CTRL + Numpad_3 : flip vertically"
                + "\n\n"
                + "The following actions can be applied to Layers"
                + "\n\n" + "- CTRL + ALT + Numpad_4 : move left"
                + "\n\n" + "- CTRL + ALT + Numpad_6 : move right"
                + "\n\n" + "- CTRL + ALT + Numpad_8 : move up"
                + "\n\n" + "- CTRL + ALT + Numpad_2 : move down");

        addHelp("Play with Meshes",
                std::string("") + "When you switch to Mesh Mode, the Mesh_Editor takes control of the Canvas. "
                "The following can actions can be performed"
                + "\n\n" + "- Move vertex : Left click + Drag and Drop"
                + "\n\n" + "- Move Line : Left click + Drag and Drop"
                + "\n\n" + "- Add Vertex : Right click on any line"
                + "\n\n" + "- Remove Vertex : Right on any Vertex"
                + "\n\n" + "- Extrude Line : SHIFT + Left click + Drag and Drop"
                + "\n\n" + "- Extrude Edge Vertex : SHIFT + Left click + Drag and Drop"
                + "\n\n" + "When a Mesh is selected it can be Moved, Scaled and Rotated"
                + "\n\n" + "- Select Mesh : CTRL + Mouse Left click"
                + "\n\n" + "- Un-select Mesh : Mouse Left on empty space"
                + "\n\n" + "- CTRL + Numpad_4 : move left"
                + "\n\n" + "- CTRL + Numpad_6 : move right"
                + "\n\n" + "- CTRL + Numpad_8 : move up"
                + "\n\n" + "- CTRL + Numpad_2 : move down"
                + "\n\n" + "- CTRL + Numpad_7 : rotate left"
                + "\n\n" + "- CTRL + Numpad_9 : rotate right"
                + "\n\n" + "- CTRL + Numpad_Multiply : zoom in"
                + "\n\n" + "- CTRL + Numpad_Divide : zoom out"
                + "\n\n"
                + "\n\n" + "Red Mesh"
                + "\n\n" + "When a Mesh becomes Red, it means that it is invalid. A red mesh will not appear when the Scene is played");

        addHelp("Play, Pause, Reset, Step",
                std::string("") + "When you want to test a Scene you can perform the followings actions"
                + "\n\n" + "- Play the Scene : press the Return key when in Object Mode"
                + "\n\n" + "- Pause the Scene : CRTL + Arrow Up when in Play Mode"
                + "\n\n" + "- Step the Scene : CRTL + Arrow Left when in Play Mode"
                + "\n\n" + "- Reset the Scene : CRTL + Arrow Left when in Play Mode");

        addHelp("The Scene Renderer",
                std::string("") + "The Scene Renderer allows to preview the final result of your game."
                + "\n\n" + "Press CTRL + Return in Object to launch the Scene Renderer");

        addHelp("The Color Picker",
            std::string("")  + "The color picker is hidden by default, it appears on the Log-View " +
            "when the Tab [Color] on the left is selected");

        addHelp("Special Action #1 : Drop Bomb",
                "In Play Mode, press B to drop a random Bomb");

        addHelp("Special Action #1 : Fire Bomb",
                std::string("") + "In Play Mode, hold SHIFT then click and drag the Mouse on the Canvas."
                + "Choose the direction you want by moving the mouse "
                + "then release the mouse to fire a Bomb");

        addHelp("Resouces",
                std::string("") + "The Engine load all resources when it starts."
                + "\n\n" + "Currently five (5) types of resource are available"
                + "\n\n" + "- Font"
                + "\n\n" + "- Texture"
                + "\n\n" + "- Animation"
                + "\n\n" + "- Sound"
                + "\n\n" + "- Music"
                + "\n\n" + "for a resource to be loaded, it just has to be place under the right folder under the [resource] folder");
    }
}
