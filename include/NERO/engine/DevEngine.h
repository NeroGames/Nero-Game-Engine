////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef DEVENGINE_H
#define DEVENGINE_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/engine/Engine.h>
#include <NERO/camera/DevCamera.h>
#include <NERO/scene/DevScene.h>
#include <NERO/scene/SceneManager.h>
#include <NERO/scene/UndoManager.h>
#include <NERO/resource/Resource.h>
#include <NERO/scene/ObjectManager.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
//SFGUI
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
#include <SFGUI/Notebook.hpp>
//BOOST
#include <boost/filesystem.hpp>
//EASY_LOG
#include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////

namespace nero
{
    class DevEngine : public Engine
    {
        public: //Method
                                    DevEngine(const sf::String& title = "Nero Game Engine", const float& winWidth = 1305.f, const float& winHeight = 660.f);
            virtual                ~DevEngine();

            template <typename T>
            void                    addScene(sf::String scene_name);

        private: //Main Methods
            void                    handleEvents();
            void                    update(const sf::Time& elapsedTime); //step_time
            void                    render();

            void                    handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                    handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

        private: //Utility
            enum Engine_Mode       {SCENE, HELP};
            enum Engine_SubMode    {OBJECT, MESH, PLAY};

        private: //Main Attributes
            //
            Engine_Mode             m_EngineMode;
            Engine_SubMode          m_EngineSubMode;

            //
            sfg::SFGUI              m_Sfgui;
            sfg::Desktop            m_Desktop;

            //
            sfg::Canvas::Ptr        m_RenderCanvas;
            sf::View                m_CanvasDefaultView;

            //
            DevCamera               m_Camera;
            ResourceManager         m_ResourceManager;
            SceneManager            m_SceneManager;

            //Utility
            struct Current
            {
                ObjectManager*      objectManager   = nullptr;
                MeshEditor*         meshEditor      = nullptr;
                UndoManager*        undoManager     = nullptr;
            };

            Current                 m_Current;

        private:
            ////////////////////////////////////////////////////////////
            //Engine_Mode
            sfg::Notebook::Ptr          m_EngineRightNotebook;
            sfg::RadioButton::Ptr       m_ObjectModeRadioButton;
            sfg::RadioButton::Ptr       m_MeshModeRadioButton;
            sfg::RadioButton::Ptr       m_PlayModeRadioButton;

            void                        onEngineRightNotebook();
            void                        onEngineSubModeButton();

            void                        set_sub_mode(Engine_SubMode mode);
            void                        switch_between_sub_mode();

            ////////////////////////////////////////////////////////////
            //Engine_Interface
            void                    build_engine();
            void                    update_engine();
            void                    update_engine_undo();
            void                    update_engine_undo_layer();
            //Left
            void                    build_utility_box(sfg::Box::Ptr main_box);
            void                    build_sprite_window(sfg::Box::Ptr main_box);
            void                    build_color_window(sfg::Window::Ptr main_window);
            void                    build_mesh_window(sfg::Window::Ptr main_window);
            //Right
            void                    build_mesh_box(sfg::Box::Ptr main_box);
            void                    build_object_box(sfg::Box::Ptr main_box);
            void                    build_grid_box(sfg::Box::Ptr main_box);
            void                    build_layer_box(sfg::Box::Ptr main_box);
            void                    build_scene_setting_box(sfg::Box::Ptr main_box);


            ////////////////////////////////////////////////////////////
            //Center_Interface
            sfg::Label::Ptr                     m_MousePositionLabel;
            sfg::Label::Ptr                     m_LogLabel;
            sf::Color                           m_RenderCanvasColor;
            bool                                m_MouseIsOnRenderCanvas;

            sfg::Button::Ptr                    m_PlayButton;
            sfg::Button::Ptr                    m_PauseButton;
            sfg::Button::Ptr                    m_StepButton;
            sfg::Button::Ptr                    m_ResetButton;
            sfg::Button::Ptr                    m_QuitButton;

            sfg::Button::Ptr                    m_UndoButton;
            sfg::Button::Ptr                    m_RedoButton;

            void                                onPlayButton();
            void                                onPauseButton();
            void                                onStepButton();
            void                                onResetButton();
            void                                onQuitButton();

            void                                onUndoButton();
            void                                onRedoButton();

            ////////////////////////////////////////////////////////////
            //Left_Interface : Utility | Sprite | Color | Mesh
            //Utility
            sfg::Button::Ptr                    m_SaveButton;
            sfg::Button::Ptr                    m_LoadButton;
            sfg::CheckButton::Ptr               m_AutoSaveChekButton;
            sf::Clock                           m_AutoSaveClock;
            sf::Time                            m_ElapseTime;
            bool                                m_AutoSave;

            void                                onSaveButton();
            void                                onLoadButton();

            //Sprite
            sf::Sprite                          m_SpritePreview;
            sf::Texture                         m_TexturePreview;

            std::function<void ()>              onSpriteButton(const std::string &label);
            std::function<void ()>              onSpriteButtonMouseEnter(const std::string &label);
            void                                onSpriteButtonMouseLeave();

            //Color
            sfg::RadioButton::Ptr               m_SpriteColorRadioButton;
            sfg::RadioButton::Ptr               m_LayerColorRadioButton;
            sfg::RadioButton::Ptr               m_CanvasColorRadioButton;

            sfg::Adjustment::Ptr                m_AlphaAdjustment;
            sfg::Adjustment::Ptr                m_RedAdjustment;
            sfg::Adjustment::Ptr                m_GreenAdjustment;
            sfg::Adjustment::Ptr                m_BlueAdjustment;

            void                                onColorRadioButton();
            void                                onColorAdjustment();

            //Mesh
            sfg::CheckButton::Ptr               m_MeshFixedRotationCheckButton;
            sfg::CheckButton::Ptr               m_MeshIsSensorCheckButton;
            sfg::CheckButton::Ptr               m_MeshAllowSleepCheckButton;

            sfg::SpinButton::Ptr                m_MeshDensitySpinButton;
            sfg::SpinButton::Ptr                m_MeshFrictionSpinButton;
            sfg::SpinButton::Ptr                m_MeshRestitutionSpinButton;
            sfg::SpinButton::Ptr                m_MeshGravityScaleSpinButton;

            void                                onMeshFixedRotationButton();
            void                                onMeshIsSensorButton();
            void                                onMeshAllowSleepButton();

            void                                onMeshDensityButton();
            void                                onMeshFrictionButton();
            void                                onMeshRestitutionButton();
            void                                onMeshGravityScaleButton();


            ////////////////////////////////////////////////////////////
            //Right_Interface : Left_Box | Scene_Setting

            //Scene_Setting
            sfg::ComboBox::Ptr                  m_SceneComboBox;

            sfg::SpinButton::Ptr                m_Gravity_x;
            sfg::SpinButton::Ptr                m_Gravity_y;

            sfg::SpinButton::Ptr                m_VelocityIterationSpinButton;
            sfg::SpinButton::Ptr                m_PositionIterationSpinButton;
            sfg::SpinButton::Ptr                m_HertzSpinButton;

            sfg::CheckButton::Ptr               m_SleepCheckButton;
            sfg::CheckButton::Ptr               m_WarmStartingCheckButton;
            sfg::CheckButton::Ptr               m_TimeOfImpactCheckButton;
            sfg::CheckButton::Ptr               m_SubSteppingCheckButton;

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


            void                                onSceneComboBox();
            void                                onScenePinButton();
            void                                onSceneCheckButton();

            //Mesh
            sfg::ComboBox::Ptr                  m_MeshComboBox;
            sfg::ComboBox::Ptr                  m_MeshTypeComboBox;

            void                                onMeshComboBox();
            void                                onMeshTypeComboBox();

            //Object
            sfg::Entry::Ptr                     m_ObjectNameEntry;
            sfg::Entry::Ptr                     m_ObjectCategoryEntry;

            void                                onObjectNameEntry();
            void                                onObjectCategoryEntry();

            //Grid
            sfg::SpinButton::Ptr                m_Grid_x_SpinButton;
            sfg::SpinButton::Ptr                m_Grid_y_SpinButton;

            //Layer
            struct Layer
            {
                                        Layer();
                sfg::Box::Ptr           get();

                sfg::ToggleButton::Ptr  seeToggleButton;
                sfg::RadioButton::Ptr   selectRadioButton;
                sfg::Entry::Ptr         nameEntry;
                int                     id;
            };

            std::vector<Layer>                  m_LayerTab;
            sfg::Box::Ptr                       m_LayerBox;

            sfg::Button::Ptr                    m_AddLayerButton;
            sfg::Button::Ptr                    m_DeleteLayerButton;
            sfg::Button::Ptr                    m_UpLayerButton;
            sfg::Button::Ptr                    m_DownLayerButton;

            void                                onAddLayerButton();
            void                                onDeleteLayerButton();
            void                                onUpLayerButton();
            void                                onDownLayerButton();

            std::function<void ()>              onSeeLayerButton(const int& id);
            std::function<void ()>              onSelectLayerButton(const int &id);
            std::function<void ()>              onLayerNameEntry(const int &id);

            void                                updateLayerBox();
            void                                updateLayerTab();
            void                                createLayer(const LayerObject& layer);

    };

    template <typename T>
    void DevEngine::addScene(sf::String scene_name)
    {
        //Add a new Scene
        m_SceneManager.registerScene<T>(scene_name);
        m_SceneComboBox->AppendItem(scene_name);
        //Create the scene directory
        using namespace boost::filesystem;
        std::string directory = NERO_FOLDER + "/" + scene_name;
        create_directories(path(directory));

        LOG(INFO) << "New Scene Registered";
        LOG(INFO) << "--> Name            : " << std::string(scene_name);
        LOG(INFO) << "--> Directory Path  : " << directory << "\n";


        //Provide the update_engine function to the Scene
        ObjectManager*  objectManager   = m_SceneManager.getObjectManager(scene_name);
        UndoManager*    undoManager     = m_SceneManager.getUndoManager(scene_name);

        objectManager->setUpdateEngineFunction(std::bind(&DevEngine::update_engine, this));
        objectManager->setEngineUndoFunction(std::bind(&DevEngine::update_engine_undo, this));

        //Load the Scene if possible
        std::string file = NERO_FOLDER + "/" + scene_name + "/" + scene_name +  ".json";

        if(exists(file))
        {
            std::ifstream stream(file);
            if(!stream.is_open())
                return;

            nlohmann::json scene;
            stream >> scene;
            stream.close();

            objectManager->loadScene(scene);

            nlohmann::json undo;
            undo["scene"] = scene;

            undoManager->add(undo);
        }
        else
        {

            nlohmann::json scene = objectManager->saveScene();
            nlohmann::json undo;

            undo["scene"] = scene;
            undoManager->add(undo);

        }

        objectManager = nullptr;
        delete objectManager;

        undoManager = nullptr;
        delete undoManager;//*/
    }
}

#endif // DEVENGINE_H
