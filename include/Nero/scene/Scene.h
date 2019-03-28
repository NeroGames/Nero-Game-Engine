////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
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
#include <Nero/object/UIObject.h>
#include <Nero/model/FrontScreen.h>
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

                                        Scene(Context context);
            virtual                    ~Scene();

            virtual void                handleEvent(const sf::Event& event);
            virtual void                update(const sf::Time& timeStep);
            virtual void                render();
            virtual void                renderShape();
            virtual void                renderFrontScreen();

        protected:
            virtual void                init();
            virtual void                checkSceneObject();
            //Collision
            virtual void                handleCollisionContactBegin(Collision collision);
            virtual void                handleCollisionContactEnd(Collision collision);
            virtual void                handleCollisionPreSolveContact(Collision collision);
            virtual void                handleCollisionPostSolveContact(Collision collision);
            //Input
            virtual void                handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            virtual void                handleMouseButtonInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            virtual void                handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
            virtual void                handleMouseWheelInput(const sf::Event::MouseWheelScrollEvent& mouse);
            //Scene
            bool                        isScenePause();
            void                        pauseScene();
            void                        resumeScene();
            void                        resetScene();
            void                        quitScene();
            void                        hideWorld();
            void                        showWorld();
            void                        hideScreen(const std::string& name);
            void                        showScreen(const std::string& name);
            void                        pushScreen(const std::string& name);
            void                        popScreen();
            bool                        isRenderEngine();
            //
            const std::string&          getSceneName() const;
            const Context&              getContext() const;
            Object::Ptr                 getWorld();
            b2World*                    getPhysicWorld();
            void                        setCanvasColor(const sf::Color& color);
            const sf::Color&            getCanvasColor() const;
            const sf::Color&            getScreenCanvasColor(const std::string& name) const;
            ObjectManager::Ptr          getObjectManager();
            SoundManager::Ptr           getSoundManager();
            ScriptManager::Ptr          getScriptManager();
            SceneSetting&               getSceneSetting();
            CameraSetting&              getCameraSetting();
            float                       getFrameRate();
            float                       getFrameTime();
            //Camera Target
            void                        setCameraTarget(Object::Ptr target);
            void                        enableFollowTaget(bool flag);
            void                        updateTargetOffset(const float left, const float right, const float up, const float down);
            //Log
            void                                                    log(const std::string& content, int level = nero::Info);
            void                                                    logIf(const std::string& content, bool condition, int level = nero::Info);
            std::function<void(const std::string&, int)>            getLog();
            std::function<void(const std::string&, bool, int)>      getLogIf();
            //resolution
            const sf::Vector2f          getResolution() const;
            void                        setResolution(const float& width, const float& height);
            //
            void                        disableLayer(const std::string& name);
            void                        enableLayer(const std::string& name);

        private:
            void                        BeginContact(b2Contact* contact);
            void                        EndContact(b2Contact* contact);
            void                        PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
            void                        PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

            void                        followTarget();

            void                        enableScreen(const std::string& name, bool flag);

        private:
            //Friend
            friend class                DestructionListener;
            friend class                BoundaryListener;
            friend class                ContactListener;
            friend class                AdvancedScene;
            friend class                SceneManager;
            friend class                RenderEngine;
            friend class                SceneRenderer;
            //Main Attribute
            std::string                 m_SceneName;
            Context                     m_Context;
            b2World*                    m_PhysicWorld;
            Object::Ptr                 m_World;
            std::vector<Screen::Ptr>    m_ScreenStack;
            std::vector<Screen::Ptr>    m_ScreenTable;
            //Manager & Setting
            ShapeRenderer               m_ShapeRenderer;
            ObjectManager::Ptr          m_ObjectManager;
            SoundManager::Ptr           m_SoundManager;
            ScriptManager::Ptr          m_ScriptManager;
            SceneSetting                m_SceneSetting;
            CameraSetting               m_CameraSetting;
            //Utility
            CameraTarget                m_CameraTarget;
            sf::Color                   m_CanvasColor;
            sf::Color                   m_SceenCanvasColor;
            bool                        m_HideWorld;
            //Other
            int32                       m_PointCount;
            sf::Text                    m_Text;
            sf::String                  m_PauseMessage;
            ContactPoint                m_Points[k_maxContactPoints];
            //FrameRate
            float                       m_FrameRate;
            float                       m_FrameTime;
            //Resolution
            sf::Vector2f                m_Resolution;
            //Callback
            std::function<void()>                           m_QuitEngine;
            std::function<void()>                           m_ResetScene;
            std::function<void(std::string, int)>           m_UpdateLog;
            std::function<void(std::string, bool, int)>     m_UpdateLogIf;

    };
}
#endif // SCENE_H
