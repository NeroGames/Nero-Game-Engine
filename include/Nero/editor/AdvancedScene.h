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
#include <Nero/core/scene/Scene.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include <memory>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
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
                std::string             name;
                SceneBuilder::Ptr       sceneBuilder;
                UndoManager::Ptr        undoManager;
            };

            struct SceneResource
            {

            };

            struct RenderContext
            {
               sf::Vector2f canvas_position;
               sf::Vector2f canvas_size;
               sf::Vector2f mouse_position;
               bool         focus;
            };

        public:
            typedef std::shared_ptr<AdvancedScene> Ptr;


        public:
                            AdvancedScene();

							void                        handleEvent(const sf::Event& event);
                            void                        update(const sf::Time& timeStep);
                             sf::RenderTexture&         render(const RenderContext& renderContext);

                             void setScene(Scene::Ptr scene);

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

            Scene::Ptr                  m_Scene;
            sf::RenderTexture           m_RenderTexture;
            RenderContext               m_RenderContext;

    };
}

#endif // ADVANCEDSCENE_H
