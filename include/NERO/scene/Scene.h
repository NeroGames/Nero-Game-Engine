#ifndef SCENE_H
#define SCENE_H

//NERO
#include <NERO/scene/Utility.h>
#include <NERO/scene/ShapeRender.h>
#include <NERO/object/Object.h>
#include <NERO/object/Collision.h>
#include <NERO/camera/Camera.h>
#include <NERO/resource/Resource.h>
#include <NERO/scene/SceneObjectManager.h>
//SFML
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
//SFGUI
#include <SFGUI/Canvas.hpp>

namespace nero
{
    class Scene : public b2ContactListener
    {
        public:
            typedef std::shared_ptr<Scene> Ptr;

            struct Context
            {
                Context(sfg::Canvas::Ptr& renderCanvas, sf::View& canvasDefaultView, Camera& camera, ResourceManager& resourceManager);

                sfg::Canvas::Ptr&   renderCanvas;
                sf::View&           canvasDefaultView;
                Camera&             camera;
                ResourceManager&    resourceManager;
            };
        //----------------------------------------------------------------------------------->
            //Constructor & destructor
                                        Scene(Context context);
            virtual                    ~Scene();
        //----------------------------------------------------------------------------------->
            //Update, render, event functions
            virtual void                handleEvent(const sf::Event& event);
            virtual void                update(SceneSettings* settings);
            virtual void                render();
            virtual void                renderShape();
            virtual void                renderOnFrontScreen();

        private:
            // Callbacks for derived classes.
            virtual void                BeginContact(b2Contact* contact);
            virtual void                EndContact(b2Contact* contact);
            virtual void                PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
            virtual void                PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        //----------------------------------------------------------------------------------->
        protected:
            virtual void                handleCollisionContactBegin(Collision collision);
            virtual void                handleCollisionContactEnd(Collision collision);
            virtual void                handleCollisionPreSolveContact(Collision collision);
            virtual void                handleCollisionPostSolveContact(Collision collision);


            virtual void                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            virtual void                handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            virtual void                handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

            virtual void                init();

            void                        setGravity(const sf::Vector2f& gravity);
            void                        setGravity(float gravity);

            void                        setCameraTarget(PhysicObject::Ptr target, float offset_left = 100.f, float offset_right = 0.f, float offset_up = 100.f, float offset_down = 100.f);
            void                        cameraFollowTarget();
            void                        setCameraFollowTarget(bool flag);

        protected:
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;

            friend class                DevScene;
            friend class                SceneManager;
            friend class                RenderEngine;


            sfg::Canvas::Ptr&           m_RenderCanvas;
            Context                     m_Context;
            ShapeRender                 m_ShapeRender;

            sf::Text                    m_Text;
            sf::String                  m_PauseMessage;

            b2World*                    m_World;
            int32                       m_PointCount;
            ContactPoint                m_Points[k_maxContactPoints];

            Object::Ptr                 m_RootObject;
            SceneObjectManager          m_ObjectManager;

            PhysicObject::Ptr           m_CameraTarget;
            float                       m_CameraTargetOffsetLeft;
            float                       m_CameraTargetOffsetRight;
            float                       m_CameraTargetOffsetUp;
            float                       m_CameraTargetOffsetDown;
            bool                        m_CameraFollowTarget;
    };
}


#endif // SCENE_H
