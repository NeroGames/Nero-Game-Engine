#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <unordered_map>
#include <memory>
//NERO
#include <NERO/resource/Resource.h>
#include <NERO/object/MeshObject.h>
#include <NERO/object/SpriteObject.h>
#include <NERO/object/LayerObject.h>
#include <NERO/scene/MeshEditor.h>
#include <NERO/scene/PhysicObjectManager.h>
//SFML
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
//SFGUI
#include <SFGUI/Canvas.hpp>



namespace nero
{
    class ObjectManager
    {
        public:
                                        ObjectManager(sfg::Canvas::Ptr& renderCanvas, TextureHolder& textureHolder);

            //Main
            void                        handleEvent(const sf::Event& event);
            void                        update(const sf::Time& elapsedTime);
            void                        render();

            //Input
            void                        handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                        handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                        handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

            //Layer
            const LayerObject::Ptr      addLayer();
            void                        deleteLayer(int id);
            void                        moveLayerUp(int id);
            void                        moveLayerDown(int id);

            void                        selectLayer(int id);
            void                        updateLayerVisibility(int id);
            void                        updateLayerName(int id, sf::String name);

            const LayerObject::Tab&     getLayerTab() const;

            //object
            enum                        Position {Up, Down, Left, Right, Up_Left, Up_Right, Down_Left, Down_Right};

            bool                        addObject(Object::Type type, const sf::String& label, sf::Vector2f position = sf::Vector2f(0.f, 0.f));
            void                        copyObject(const Position& position);
            void                        deleteObject(Object::Ptr object);
            void                        setObjectName(const sf::String& name);
            void                        setObjectCategory(const sf::String& category);
            Object::Ptr                 getSelectedObject();



            //Sprite
            void                        updateLayerColor(const sf::Color& color);
            void                        updateAllLayerAlpha(int alpha);
            void                        updateSpriteColor(const sf::Color& color);
            sf::Color                   getLayerColor();


            //Mesh
            void                        setMeshType(const sf::String& label);
            void                        setMeshFixedRotation(bool tag);
            void                        setMeshSensor(bool tag);
            void                        setMeshAllowSleep(bool tag);
            void                        setMeshDensity(float density);
            void                        setMeshFriction(float friction);
            void                        setMeshRestitution(float restitution);
            void                        setMeshGravityScale(float gravityScale);

            MeshEditor*                 getMeshEditor();

            //Scene
            void                        buildScene(Object::Ptr mainObject);
            void                        destroyAllPhysicObject(Object::Ptr mainObject);
            void                        updateLayerOrder();
            nlohmann::json              saveScene();
            void                        loadScene(nlohmann::json scene);



            void                        setUpdateEngineFunction(std::function<void()>  f);
            void                        setEngineUndoFunction(std::function<void()>  f);

            void                        setWorld(b2World* world);



        private:
            Object::Ptr                     findObject(Object::Ptr object, sf::Vector2f pos);

            void                            findObject(Object::Ptr object, int id, Object::Ptr& result);
            void                            findObject(Object::Ptr object, sf::String name, Object::Ptr& result);
            void                            findObject(Object::Ptr object, sf::Vector2f pos, Object::Ptr& result);

        public:
            Object::Ptr                     findObject(Object::Ptr object, sf::String name);
            Object::Ptr                     findObject(Object::Ptr object, int id);

        private:
            SpriteObject::Ptr               loadSprite(nlohmann::json& json);
            MeshObject::Ptr                 loadMesh(nlohmann::json& json);
            int                             getNewId();

        private:
            sfg::Canvas::Ptr&               m_RenderCanvas;
            TextureHolder&                  m_TextureHolder;
            sf::RectangleShape              m_SelectionRect;
            sf::Vector2f                    m_LastMousePosition;

            float                           m_PanningSpeed;
            float                           m_RotationSpeed;
            float                           m_ZoomingRatio;

            LayerObject::Tab                m_LayerTab;
            LayerObject::Ptr                m_SelectedLayer;
            int                             m_LayerCount;

            Object::Ptr                     m_SelectedObject;
            int                             m_ObjectCount;

            MeshEditor                      m_MeshEditor;
            PhysicObjectManager             m_PhysicObjectManager;

            b2World*                        m_World;

            std::function<void()>           m_UpdateEngine;
            std::function<void()>           m_EngineUndo;
    };
}
#endif // OBJECTMANAGER_H
