////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef WorldBuilder_H
#define WorldBuilder_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/resource/ResourceManager.h>
#include <Nero/core/cpp/object/PhysicsMeshObject.h>
#include <Nero/core/cpp/object/SpriteObject.h>
#include <Nero/core/cpp/object/ButtonObject.h>
#include <Nero/core/cpp/object/AnimationObject.h>
#include <Nero/core/cpp/object/LayerObject.h>
#include <Nero/core/cpp/object/TextObject.h>
#include <Nero/core/cpp/object/UIObject.h>
#include <Nero/core/cpp/scene/MeshEditor.h>
#include <Nero/core/cpp/scene/PhysicsManager.h>
#include <Nero/core/cpp/scene/SceneUtility.h>
#include <Nero/core/cpp/object/LightObject.h>
#include <Nero/editor/icon/LightIcon.h>
// SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
// Std
#include <unordered_map>
#include <memory>
#include <Nero/core/cpp/scene/UndoManager.h>
#include <ltbl/LightSystem.hpp>

////////////////////////////////////////////////////////////
namespace nero
{
    class WorldBuilder
    {
      public:
        typedef std::shared_ptr<WorldBuilder>      Ptr;
        typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;
        typedef std::shared_ptr<ltbl::LightSystem> LightManagerPtr;

      public:
        WorldBuilder();

        void init();

        // Main
        void handleEvent(const sf::Event& event);
        void update(const sf::Time& elapsedTime);
        void render();
        // Input
        void handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
        void handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                     const bool&                        isPressed);
        void handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
        // Layer
        const LayerObject::Ptr  addLayer();
        void                    deleteLayer(int id);
        void                    moveLayerUp(int id);
        void                    moveLayerDown(int id);
        bool                    mergeLayerUp(int id);
        bool                    mergeLayerDown(int id);

        void                    selectLayer(int id);
        void                    setSelectedLayer(LayerObject::Ptr layerObject);
        void                    setSelectedObject(Object::Ptr object);
        void                    updateLayerVisibility(int id);
        void                    updateLayerName(int id, sf::String name);

        const LayerObject::Tab& getLayerTable() const;
        // object
        enum Position
        {
            Up,
            Down,
            Left,
            Right,
            Up_Left,
            Up_Right,
            Down_Left,
            Down_Right
        };

        void             addObject(Object::Type      type,
                                   const sf::String& label,
                                   sf::Vector2f      position = sf::Vector2f(0.f, 0.f));
        void             copyObject(const Position& position);
        void             deleteObject(Object::Ptr object);
        void             setObjectName(const sf::String& name);
        void             setObjectCategory(const sf::String& category);
        void             moveObjectUp();
        void             moveObjectDown();
        Object::Ptr      getSelectedObject();
        LayerObject::Ptr getSelectedLayer();
        // Sprite
        void             updateLayerColor(const sf::Color& color);
        void             updateAllLayerAlpha(int alpha);
        void             updateSpriteColor(const sf::Color& color);
        void             updateTextColor(const sf::Color& color);
        void             updateOutlineTextColor(const sf::Color& color);
        sf::Color        getLayerColor();
        //
        MeshEditor::Ptr  getMeshEditor();
        // Scene
        void             buildScene(Object::Ptr rootObject);
        Object::Ptr      buildScene(std::shared_ptr<ltbl::LightSystem> lightManager,
                                    std::shared_ptr<b2World>           physicsWorld);
        void             destroyAllPhysicObject(Object::Ptr mainObject);
        void             updateLayerOrder();
        nlohmann::json   saveScene();
        void             loadScene(nlohmann::json scene);

        void             setUpdateUndo(std::function<void()> fn);

        void             setPhysicWorld(std::shared_ptr<b2World> world);

        void             buildUI(UIObject::Ptr rootObject);

        void             setTextContent(const sf::String& content);
        void             setTextFont(const sf::String& font);
        void             setTextFontSize(float value);
        void             setTextLetterSpacing(float value);
        void             setTextLineSpacing(float value);
        void             setTextOutlineThickness(float value);
        void             setTextStyle(bool bold, bool italic, bool underLine, bool lineThrough);

        const sf::Color& getCanvasColor() const;
        void             setCanvasColor(const sf::Color& color);

        int              getObjectCount();

        void             setRenderContext(const RenderContext::Ptr& renderContext);
        void             setRenderTexture(const RenderTexturePtr& renderTexture);
        void             setResourceManager(const ResourceManager::Ptr& resourceManager);

      private:
        Object::Ptr findObject(Object::Ptr object, sf::Vector2f pos);

        void        findObject(Object::Ptr object, int id, Object::Ptr& result);
        void        findObject(Object::Ptr object, sf::String name, Object::Ptr& result);
        void        findObject(Object::Ptr object, sf::Vector2f pos, Object::Ptr& result);

      public:
        Object::Ptr findObject(Object::Ptr object, sf::String name);
        Object::Ptr findObject(Object::Ptr object, int id);

      private:
        SpriteObject::Ptr      loadSprite(nlohmann::json& json);
        TextObject::Ptr        loadText(nlohmann::json& json);
        PhysicsMeshObject::Ptr loadMesh(nlohmann::json& json);
        AnimationObject::Ptr   loadAnimation(nlohmann::json& json);
        LightIcon::Ptr         loadLight(nlohmann::json& json);
        int                    getNewId();

      private:
        ResourceManager::Ptr     m_ResourceManager;
        sf::RectangleShape       m_SelectionRect;
        sf::Vector2f             m_LastMousePosition;

        float                    m_PanningSpeed;
        float                    m_RotationSpeed;
        float                    m_ZoomingRatio;

        LayerObject::Tab         m_LayerTable;
        LayerObject::Ptr         m_SelectedLayer;
        int                      m_LayerCount;

        Object::Ptr              m_SelectedObject;
        int                      m_ObjectCount;

        MeshEditor::Ptr          m_MeshEditor;
        PhysicsManager           m_PhysicsManager;

        std::shared_ptr<b2World> m_PhysicWorld;

        SceneSetting             m_SceneSetting;
        sf::Color                m_CanvasColor;

        RenderTexturePtr         m_RenderTexture;
        RenderContext::Ptr       m_RenderContext;
        bool                     m_RightSelection;
        bool                     m_ClickedObject;
        sf::Texture              m_LightTexture;
        bool                     m_LeftSelection;
        std::function<void()>    m_UpdateUndo;
    };
} // namespace nero
#endif // WorldBuilder_H
