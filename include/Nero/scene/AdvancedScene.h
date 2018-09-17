////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/Scene.h>
#include <Nero/scene/SceneBuilder.h>
#include <Nero/scene/UndoManager.h>
#include <Nero/camera/AdvancedCamera.h>
#include <Nero/scene/MeshEditor.h>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
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

        public:
                                        AdvancedScene(Context context);
            virtual                    ~AdvancedScene();

        public:
            virtual void                update(const sf::Time& timeStep);
            void                        renderDebug();

        private:
            //Main function
            void                        init();
            void                        setScene(Scene::Ptr scene);
            SceneBuilder::Ptr           getSceneBuilder();
            UndoManager::Ptr            getUndoManager();
            //
            void                        shiftOrigin(const b2Vec2& newOrigin);
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

        protected:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                SceneManager;
            //Scene
            Scene::Ptr                  m_Scene;
            //Manager
            SceneBuilder::Ptr           m_SceneBuilder;
            UndoManager::Ptr            m_UndoManger;
            DestructionListener         m_DestructionListener;
            //
            SceneSetting                m_SceneSetting;
            CameraSetting               m_CameraSetting;
            SoundSetting                m_SoundSetting;
            //
            std::string                 m_SceneName;
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
    };
}
#endif // ADVANCEDSCENE_H
