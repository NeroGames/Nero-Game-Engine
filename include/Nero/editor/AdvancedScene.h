#ifndef ADVANCEDSCENE_H
#define ADVANCEDSCENE_H

#include <vector>
#include <memory>
//Box2D
#include <Box2D/Dynamics/Joints/b2MouseJoint.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Dynamics/b2TimeStep.h>
#include <Nero/editor/SceneBuilder.h>
#include <Nero/editor/UndoManager.h>

namespace nero
{
    //Forward declaration
    class AdvancedScene;

    //This is called when a joint in the world is implicitly destroyed because an attached body is destroyed.
    //This gives us a chance to nullify the mouse joint.
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
            struct SceneTrunk
            {
                std::string         name;
                SceneBuilder::Ptr   sceneBuilder;
                UndoManager::Ptr    undoManager;
            };

            struct SceneScreen
            {
                SceneBuilder::Ptr           m_SceneBuilder;
                UndoManager::Ptr            m_UndoManager;
            };

            struct SceneResource
            {

            };

        public:
            AdvancedScene();

        private:

            void                        init();



        private:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                SceneManager;
            //Destruction Listener
            DestructionListener         m_DestructionListener;



            std::vector<SceneScreen>    m_SceneScreenTable;
            std::vector<SceneTrunk>     m_SceneTrunkTable;
            //other
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
