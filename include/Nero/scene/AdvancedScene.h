////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2022 sk-landry
/////////////////////////////////////////////////////////////
#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/Scene.h>
#include <Nero/scene/SceneBuilder.h>
#include <Nero/scene/UndoManager.h>
#include <Nero/scene/SoundManager.h>
#include <Nero/camera/AdvancedCamera.h>
#include <Nero/scene/MeshEditor.h>
#include <Nero/model/Grid.h>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
//STD
#include <algorithm>
////////////////////////////////////////////////////////////
namespace nero
{
    //Forward declaration
    class AdvancedScene;

    ///\brief This is called when a joint in the world is implicitly destroyed because an attached body is destroyed. This gives us a chance to nullify the mouse joint.
    class DestructionListener : public b2DestructionListener
    {
        public:
            void    SayGoodbye(b2Fixture* fixture);
            void    SayGoodbye(b2Joint* joint);

           std::shared_ptr<AdvancedScene>  scene;
    };

    class AdvancedScene
    {
        public:
            typedef std::shared_ptr<AdvancedScene> Ptr;

                                        AdvancedScene(Scene::Context context);
            virtual                    ~AdvancedScene();

            virtual void                update(const sf::Time& timeStep);
            void                        renderDebug();

        private:
            //Main function
            void                        init();
            void                        setScene(Scene::Ptr scene);
            SceneBuilder::Ptr           getSceneBuilder();
            UndoManager::Ptr            getUndoManager();
            SceneBuilder::Ptr           getScreenBuilder();
            UndoManager::Ptr            getScreenUndoManager();
            SoundManager::Ptr           getSoundManager();
            Grid::Ptr                   getFrontScreenGrid();
            CameraSetting&              getScreenCameraSetting();
            sf::Color                   getScreenCanvasColor();


            //
            //Mouse
            void                        shiftMouseDown(const b2Vec2& p);
            virtual void                mouseDown(const b2Vec2& p);
            virtual void                mouseUp(const b2Vec2& p);
            void                        mouseMove(const b2Vec2& p);
            //Bomb
            void                        launchBomb();
            void                        launchBomb(const b2Vec2& position, const b2Vec2& velocity);
            void                        spawnBomb(const b2Vec2& worldPt);
            void                        completeBombSpawn(const b2Vec2& p);
            // Let derived tests know that a joint was destroyed.
            virtual void                jointDestroyed(b2Joint* joint);
            void                        setName(std::string name);

            bool                            addScreen(const std::string& name, bool loading = false);
            bool                            loadScreen(const std::string& name);
            void                            selectScreen(const std::string& name);
            std::vector<std::string>        getScreenTable();

            bool                            deleteScreen(const std::string& name);
            bool                            renameScreen(const std::string& name, const std::string& newName);
            void                            setScreenCanvasColor(const sf::Color& color);
            void                            destroyBomb();

            void                            setUpdateUI(std::function<void()>  fn);
            void                            setUpdateUndo(std::function<void()>  fn);
            void                            setUpdateLog(std::function<void(const std::string&, int)>  fn);
            void                            setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn);


        private:
            struct FrontScreen
            {
                SceneBuilder::Ptr           screenBuilder       = nullptr;
                UndoManager::Ptr            undoManager         = nullptr;
                Grid::Ptr                   grid                = nullptr;
                std::string                 name                = "Screen";
                sf::Color                   canvasColor         = sf::Color::Black;
                CameraSetting               cameraSetting;
            };

        protected:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                SceneManager;
            //Destruction Listener
            DestructionListener         m_DestructionListener;
            //Scene
            Scene::Ptr                  m_Scene;
            Scene::Context              m_Context;
            SceneSetting                m_SceneSetting;
            SoundManager::Ptr           m_SoundManager;
            std::string                 m_SceneName;
            //World
            SceneBuilder::Ptr           m_SceneBuilder;
            UndoManager::Ptr            m_UndoManager;
            Grid::Ptr                   m_Grid;
            CameraSetting               m_CameraSetting;
            sf::Color                   m_CanvasColor;
            //Screen
            std::vector<FrontScreen>    m_FrontScreenTable;
            std::string                 m_CurrentScreen;
            //Other
            sf::String                  m_Message;
            sf::String                  m_StatMessage;
            sf::String                  m_ProfileMessage;
            b2Body*                     m_Bomb;
            b2Vec2                      m_BombSpawnPoint;
            bool                        m_BombSpawning;
            b2Body*                     m_GroundBody;
            b2AABB                      m_WorldAABB;
            b2MouseJoint*               m_MouseJoint;
            b2Vec2                      m_MouseWorld;
            int32                       m_StepCount;
            b2Profile                   m_MaxProfile;
            b2Profile                   m_TotalProfile;

            std::function<void()>                               m_UpdateUI;
            std::function<void()>                               m_UpdateUndo;
            std::function<void(const std::string&, int)>        m_UpdateLog;
            std::function<void(const std::string&, bool, int)>  m_UpdateLogIf;
    };
}
#endif // ADVANCEDSCENE_H
