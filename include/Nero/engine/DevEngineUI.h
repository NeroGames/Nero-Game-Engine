////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef DEVENGINEUI_H_INCLUDED
#define DEVENGINEUI_H_INCLUDED
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/resource/ResourceManager.h>
#include <Nero/scene/SceneManager.h>
#include <Nero/utility/EnumUtil.h>
#include <Nero/model/Grid.h>
#include <Nero/engine/EngineSetting.h>
//SMFL
#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
//SFGUI
#include <SFGUI/Window.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Frame.hpp>
#include <SFGUI/Separator.hpp>
#include <SFGUI/ScrolledWindow.hpp>
#include <SFGUI/Notebook.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Window.hpp>
#include <SFGUI/SpinButton.hpp>
#include <SFGUI/CheckButton.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Scale.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    class DevEngineUI
    {
        public:
            typedef std::unique_ptr<DevEngineUI>    Ptr;
            friend class                            DevEngine;

        public:
                                        DevEngineUI(sf::RenderWindow& window, ResourceManager::Ptr resourceManager);

            //EngineUI
            void                        build();
            void                        handleEvent(sf::Event& event);
            void                        update(const sf::Time& timeStep);
            void                        render();
            //Get Engine Resources
            void                        setSceneManager(SceneManager::Ptr sceneManager);
            void                        setSoundManager(SoundManager::Ptr soundManager);
            void                        setEngineSetting(EngineSetting engineSetting);
            void                        setCamera(AdvancedCamera::Ptr camera);
            void                        setGrid(Grid::Ptr grid);
            //
            void                        addScene(std::string name);
            void                        selectScene(std::string name);
            sf::View&                   getCanvasFrontView();
            sfg::Canvas::Ptr            getRenderCanvas() const;


        private:
            void                        handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                        handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
            void                        handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);

            void                        updatePositionLabel();
            void                        updateSceneSetting();
            void                        updateCameraSetting();
            void                        autoSave();


        private:
            ////////////////////////////////////////////////////////////
            sfg::SFGUI                          m_Sfgui;
            sfg::Desktop                        m_Desktop;
            sf::RenderWindow&                   m_Window;
            //Manager
            EngineSetting                       m_EngineSetting;
            ResourceManager::Ptr                m_ResourceManager;
            SceneManager::Ptr                   m_SceneManager;
            SoundManager::Ptr                   m_SoundManager;
            SceneBuilder::Ptr                   m_ActiveSceneBuilder;
            MeshEditor::Ptr                     m_ActiveMeshEditor;
            UndoManager::Ptr                    m_ActiveUndoManager;
            SoundManager::Ptr                   m_ActiveSoundManager;
            //Camera
            AdvancedCamera::Ptr                 m_Camera;
            //Engine mode
            EngineMode                          m_EngineMode;
            EngineSubMode                       m_EngineSubMode;
            //Grid
            Grid::Ptr                           m_Grid;
            //Callback
            void                                updateUI();
            void                                updateUndo();
            void                                updateLayerUndo();
            void                                updateLog(const std::string& message);
            ////////////////////////////////////////////////////////////
            //Left_Interface : Utility | Music | Grid | Sprite | Animation | Color | Mesh
            void                                build_left(sfg::Box::Ptr left_box);
            void                                build_utility_box(sfg::Box::Ptr utility_box);
            void                                build_music_box(sfg::Box::Ptr music_box);
            void                                build_grid_box(sfg::Box::Ptr grid_box);
            void                                build_sprite_box(sfg::Box::Ptr sprite_box);
            void                                build_animation_box(sfg::Box::Ptr animation_box);
            void                                build_color_window(sfg::Window::Ptr color_window);
            void                                build_mesh_window(sfg::Window::Ptr mesh_window);
            //Utility
            sfg::RadioButton::Ptr               m_ObjectModeRadioButton;
            sfg::RadioButton::Ptr               m_MeshModeRadioButton;
            sfg::RadioButton::Ptr               m_PlayModeRadioButton;
            sfg::Button::Ptr                    m_SaveButton;
            sfg::Button::Ptr                    m_LoadButton;
            sfg::CheckButton::Ptr               m_AutoSaveChekButton;
            sf::Clock                           m_AutoSaveClock;
            sf::Time                            m_ElapseTime;
            //Music
            sfg::ComboBox::Ptr                  m_MusicComboBox;
            sfg::ComboBox::Ptr                  m_SoundComboBox;
            sfg::Adjustment::Ptr                m_MusicAdjustment;
            sfg::Adjustment::Ptr                m_SoundAdjustment;
            sfg::Scale::Ptr                     m_MusicScaleBar;
            sfg::Scale::Ptr                     m_SoundScaleBar;
            //Grid
            sfg::SpinButton::Ptr                m_xGridSpinButton;
            sfg::SpinButton::Ptr                m_yGridSpinButton;
            //Sprite | Animation
            sf::Sprite                          m_Preview;
            sf::Texture                         m_PreviewTexture;
            //Color
            sfg::RadioButton::Ptr               m_SpriteColorRadioButton;
            sfg::RadioButton::Ptr               m_AnimationColorRadioButton;
            sfg::RadioButton::Ptr               m_LayerColorRadioButton;
            sfg::RadioButton::Ptr               m_CanvasColorRadioButton;
            sfg::Adjustment::Ptr                m_AlphaAdjustment;
            sfg::Adjustment::Ptr                m_RedAdjustment;
            sfg::Adjustment::Ptr                m_GreenAdjustment;
            sfg::Adjustment::Ptr                m_BlueAdjustment;
            //Mesh
            sfg::CheckButton::Ptr               m_MeshFixedRotationCheckButton;
            sfg::CheckButton::Ptr               m_MeshIsSensorCheckButton;
            sfg::CheckButton::Ptr               m_MeshAllowSleepCheckButton;
            sfg::SpinButton::Ptr                m_MeshDensitySpinButton;
            sfg::SpinButton::Ptr                m_MeshFrictionSpinButton;
            sfg::SpinButton::Ptr                m_MeshRestitutionSpinButton;
            sfg::SpinButton::Ptr                m_MeshGravityScaleSpinButton;
            //Sequence
            sfg::Box::Ptr                       m_SequenceBox;
            sfg::Notebook::Ptr                  m_SequenceNoteBook;
            //Callback
            std::function<void ()>              onSpriteButton(const std::string &label);
            std::function<void ()>              onSpriteButtonMouseOver(const std::string &label);
            std::function<void ()>              onAnimationButton(const std::string &label);
            std::function<void ()>              onAnimationButtonMouseOver(const std::string &label);
            void                                onClosePreview();
            void                                onMeshFixedRotationButton();
            void                                onMeshIsSensorButton();
            void                                onMeshAllowSleepButton();
            void                                onMeshDensityButton();
            void                                onMeshFrictionButton();
            void                                onMeshRestitutionButton();
            void                                onMeshGravityScaleButton();
            void                                onColorRadioButton();
            void                                onColorAdjustment();
            void                                onSaveButton();
            void                                onLoadButton();
            void                                onEngineSubModeButton();
            void                                setEngineSubMode(EngineSubMode mode);
            void                                switchEngineSubMode();
            void                                onPlayMusic();
            void                                onPlaySound();
            void                                onStopMusic();
            void                                onMusicAdjustment();
            void                                onSoundAdjustment();
            ////////////////////////////////////////////////////////////
            //Center_Interface : Mouse-Camera | Button | Canvas | Log
            void                                build_center(sfg::Box::Ptr main_box);
            //Mouse
            std::string                         m_MousePositionText;
            sfg::Label::Ptr                     m_PositionLabel;
            sfg::Label::Ptr                     m_LogLabel;
            //Button
            sfg::Button::Ptr                    m_PlayButton;
            sfg::Button::Ptr                    m_PauseButton;
            sfg::Button::Ptr                    m_StepButton;
            sfg::Button::Ptr                    m_ResetButton;
            sfg::Button::Ptr                    m_QuitButton;
            sfg::Button::Ptr                    m_UndoButton;
            sfg::Button::Ptr                    m_RedoButton;
            //Canvas
            sfg::Canvas::Ptr                    m_RenderCanvas;
            sf::Color                           m_CanvasColor;
            sf::View                            m_CanvasFrontView;
            bool                                m_MouseOnCanvas;
            //Log
            sfg::Adjustment::Ptr                m_LogAdjustment;
            //Callback
            void                                onPlayButton();
            void                                onPauseButton();
            void                                onStepButton();
            void                                onResetButton();
            void                                onQuitButton();
            void                                onUndoButton();
            void                                onRedoButton();
            ////////////////////////////////////////////////////////////
            //Right_Interface : Mesh | Object | Layer | Scene | Draw
            void                                build_right(sfg::Box::Ptr main_box);
            void                                build_mesh_box(sfg::Box::Ptr main_box);
            void                                build_object_box(sfg::Box::Ptr main_box);
            void                                build_layer_box(sfg::Box::Ptr main_box);
            void                                build_scene_box(sfg::Box::Ptr main_box);
            //Engine Mode
            sfg::Notebook::Ptr                  m_EngineModeNotebook;
            //Layer UI
            struct LayerUI
            {
                                                LayerUI();
                sfg::Box::Ptr                   getLayerBox();

                sfg::ToggleButton::Ptr          showToggleButton;
                sfg::RadioButton::Ptr           selectRadioButton;
                sfg::Entry::Ptr                 nameEntry;
                int                             layerId;
            };
            //Mesh
            sfg::ComboBox::Ptr                  m_MeshComboBox;
            sfg::ComboBox::Ptr                  m_MeshTypeComboBox;
            //Object
            sfg::Entry::Ptr                     m_ObjectNameEntry;
            sfg::Entry::Ptr                     m_ObjectCategoryEntry;
            //Scene
            sfg::ComboBox::Ptr                  m_SceneComboBox;
            sfg::SpinButton::Ptr                m_xGravitySpinButton;
            sfg::SpinButton::Ptr                m_yGravitySpinButton;
            sfg::SpinButton::Ptr                m_VelocityIterationSpinButton;
            sfg::SpinButton::Ptr                m_PositionIterationSpinButton;
            sfg::SpinButton::Ptr                m_HertzSpinButton;
            sfg::CheckButton::Ptr               m_SleepCheckButton;
            sfg::CheckButton::Ptr               m_WarmStartingCheckButton;
            sfg::CheckButton::Ptr               m_TimeOfImpactCheckButton;
            sfg::CheckButton::Ptr               m_SubSteppingCheckButton;
            //Draw
            sfg::CheckButton::Ptr               m_DrawAxisCheckButton;
            sfg::CheckButton::Ptr               m_DrawGridCheckButton;
            sfg::CheckButton::Ptr               m_DrawShapesCheckButton;
            sfg::CheckButton::Ptr               m_DrawJointsCheckButton;
            sfg::CheckButton::Ptr               m_DrawAABBsCheckButton;
            sfg::CheckButton::Ptr               m_DrawContactPointsCheckButton;
            sfg::CheckButton::Ptr               m_DrawContactNormalsCheckButton;
            sfg::CheckButton::Ptr               m_DrawContactImpulsesCheckButton;
            sfg::CheckButton::Ptr               m_DrawFrictionImpulsesCheckButton;
            sfg::CheckButton::Ptr               m_DrawCenterOfMassesCheckButton;
            sfg::CheckButton::Ptr               m_DrawStatisticsCheckButton;
            sfg::CheckButton::Ptr               m_DrawProfileCheckButton;
            //Layer
            std::vector<LayerUI>                m_LayerTable;
            sfg::Box::Ptr                       m_LayerBox;
            sfg::Button::Ptr                    m_AddLayerButton;
            sfg::Button::Ptr                    m_DeleteLayerButton;
            sfg::Button::Ptr                    m_UpLayerButton;
            sfg::Button::Ptr                    m_DownLayerButton;
            sfg::Button::Ptr                    m_MergeUpLayerButton;
            sfg::Button::Ptr                    m_MergeDownLayerButton;
            sfg::Button::Ptr                    m_ObjectUpLayerButton;
            sfg::Button::Ptr                    m_ObjectDownLayerButton;
            sfg::Notebook::Ptr                  m_LayerNoteBook;
            //Callback
            void                                onEngineModeNotebook();
            void                                onSceneSelection();
            void                                onScenePinButton();
            void                                onSceneCheckButton();
            void                                onMeshComboBox();
            void                                onMeshTypeComboBox();
            void                                onObjectNameEntry();
            void                                onObjectCategoryEntry();
            void                                onAddLayerButton();
            void                                onDeleteLayerButton();
            void                                onUpLayerButton();
            void                                onDownLayerButton();
            void                                onMergeUpLayerButton();
            void                                onMergeDownLayerButton();
            void                                onObjectUpLayerButton();
            void                                onObjectDownLayerButton();
            std::function<void ()>              onShowLayerButton(const int& layerId);
            std::function<void ()>              onSelectLayerButton(const int& layerId);
            std::function<void ()>              onLayerNameEntry(const int& layerId);
            void                                updateLayerBox();
            void                                updateLayerTable();
            void                                createLayer(LayerObject::Ptr layerObject);
    };
}

#endif // DEVENGINEUI_H_INCLUDED
