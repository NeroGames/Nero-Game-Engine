////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef SCENE_H
#define SCENE_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/scene/SceneUtil.h>
#include <Nero/scene/SoundManager.h>
#include <Nero/scene/ScriptManager.h>
#include <Nero/scene/ShapeRenderer.h>
#include <Nero/scene/ObjectManager.h>
#include <Nero/model/Collision.h>
#include <Nero/object/Object.h>
//SFGUI
#include <SFGUI/Canvas.hpp>
//SFML
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
//LUA
#include <lua/lua.hpp>
////////////////////////////////////////////////////////////
namespace nero
{
    class Scene : public b2ContactListener
    {
        public:
            typedef std::shared_ptr<Scene> Ptr;

        public:
                                        Scene(Context context);
            virtual                    ~Scene();

        public:
            virtual void                handleEvent(const sf::Event& event);
            virtual void                update(const sf::Time& timeStep);
            virtual void                render();
            virtual void                renderShape();
            virtual void                renderFrontScreen();

        private:
            virtual void                beginContact(b2Contact* contact);
            virtual void                endContact(b2Contact* contact);
            virtual void                preSolve(b2Contact* contact, const b2Manifold* oldManifold);
            virtual void                postSolve(b2Contact* contact, const b2ContactImpulse* impulse);

        protected:
            virtual void                init();
            //Collision
            virtual void                handleCollisionContactBegin(Collision collision);
            virtual void                handleCollisionContactEnd(Collision collision);
            virtual void                handleCollisionPreSolveContact(Collision collision);
            virtual void                handleCollisionPostSolveContact(Collision collision);
            //Input
            virtual void                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            virtual void                handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            virtual void                handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
            void                        handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);
            //Gravity
            void                        setGravity(const sf::Vector2f& gravity);
            void                        setGravity(float gravity);
            //Camera Target
            void                        setCameraTarget(PhysicObject::Ptr target);
            void                        setCameraTargetOffset(const float left, const float right, const float up, const float down);
            void                        setCameraFollowTarget(bool flag);
            void                        cameraFollowTarget();

        protected:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                AdvancedScene;
            friend class                SceneManager;
            friend class                RenderEngine;
            //Main attribute
            Context                     m_Context;
            sfg::Canvas::Ptr            m_RenderCanvas;
            b2World*                    m_PhysicWorld;
            Object::Ptr                 m_RootObject;
            PhysicObject::Ptr           m_CameraTarget;
            std::string                 m_SceneName;
            lua_State*                  L;
            //Setting
            SceneSetting                m_SceneSetting;
            CameraSetting               m_CameraSetting;
            SoundSetting                m_SoundSetting;
            CameraTargetOffset          m_CameraTargetOffset;
            //Manager
            ShapeRenderer               m_ShapeRenderer;
            ObjectManager::Ptr          m_ObjectManager;
            SoundManager::Ptr           m_SoundManager;
            ScriptManager::Ptr          m_ScriptManager;
            //Utility
            int32                       m_PointCount;
            sf::Text                    m_Text;
            sf::String                  m_PauseMessage;
            ContactPoint                m_Points[k_maxContactPoints];
            bool                        m_CameraFollowTarget;
    };
}
#endif // SCENE_H
