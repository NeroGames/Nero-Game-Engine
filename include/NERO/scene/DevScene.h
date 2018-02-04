#ifndef DEVSCENE_H
#define DEVSCENE_H

//NERO
#include <NERO/scene/Scene.h>
#include <NERO/scene/ObjectManager.h>
#include <NERO/scene/UndoManager.h>
#include <NERO/camera/DevCamera.h>
#include <NERO/scene/MeshEditor.h>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>

namespace nero
{
    //Forward declaration
    class DevScene;

    // This is called when a joint in the world is implicitly destroyed because an attached body is destroyed. This gives us a chance to nullify the mouse joint.
    class DestructionListener : public b2DestructionListener
    {
        public:
            void    SayGoodbye(b2Fixture* fixture);
            void    SayGoodbye(b2Joint* joint);

            DevScene*  scene;
    };

    class DevScene
    {
        public:
            typedef std::shared_ptr<DevScene> Ptr;

                                        DevScene(Scene::Context context);
            virtual                    ~DevScene();

            virtual void                update(SceneSettings* settings);
            void                        renderDebug();

        //----------------------------------------------------------------------------------->
            //Mouse fonctions
            void                        ShiftMouseDown(const b2Vec2& p);
            virtual void                MouseDown(const b2Vec2& p);
            virtual void                MouseUp(const b2Vec2& p);
            void                        MouseMove(const b2Vec2& p);

            //Bomb fonctions
            void                        LaunchBomb();
            void                        LaunchBomb(const b2Vec2& position, const b2Vec2& velocity);
            void                        SpawnBomb(const b2Vec2& worldPt);
            void                        CompleteBombSpawn(const b2Vec2& p);

            // Let derived tests know that a joint was destroyed.
            virtual void                JointDestroyed(b2Joint* joint) { B2_NOT_USED(joint); }

            //
            void                        ShiftOrigin(const b2Vec2& newOrigin);
        //----------------------------------------------------------------------------------->
            ObjectManager*              getObjectManager();
            UndoManager*                getUndoManager();

            void                        setName(const std::string& name);
            std::string                 getName();

            void                        setCameraSettings(CameraSettings cameraSettings);
            CameraSettings&             getCameraSettings();

            void                        setScene(Scene::Ptr scene);

            void                        init();

        protected:
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;

            friend class                SceneManager;

            sf::String                  m_Message;
            sf::String                  m_StatMessage;
            sf::String                  m_ProfileMessage;

            ObjectManager               m_ObjectManager;
            UndoManager                 m_UndoManger;

            std::string                 m_Name;

            SceneSettings               m_Settings;
            b2Body*                     m_GroundBody;
            b2AABB                      m_WorldAABB;

            b2MouseJoint*               m_MouseJoint;
            b2Vec2                      m_MouseWorld;


            b2Body*                     m_Bomb;
            b2Vec2                      m_BombSpawnPoint;
            bool                        m_BombSpawning;

            int32                       m_StepCount;

            DestructionListener         m_DestructionListener;

            b2Profile                   m_MaxProfile;
            b2Profile                   m_TotalProfile;

            CameraSettings              m_CameraSettings;
            Scene::Ptr                  m_Scene;
    };
}

#endif // DEVSCENE_H
