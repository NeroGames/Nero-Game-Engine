////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MESHEDITOR_H
#define MESHEDITOR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicsMeshObject.h>
#include <Nero/core/cpp/model/PolygonMesh.h>
#include <Nero/core/cpp/model/CircleMesh.h>
#include <Nero/core/cpp/scene/SceneUtility.h>
#include <SFML/Graphics/RenderTexture.hpp>
// Std
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class MeshEditor
    {
      public:
        typedef std::shared_ptr<MeshEditor>         Ptr;
        typedef std::shared_ptr<sf::RenderTexture>  RenderTexturePtr;
        typedef sf::RectangleShape                  Vertex;
        typedef std::vector<PhysicsMeshObject::Ptr> MeshTable;

        MeshEditor();
        virtual ~MeshEditor();

        void handleEvent(const sf::Event& event);

        void addMesh(const PhysicsMeshObject::Ptr& meshObject);
        void deleteMesh(const int& id);
        void clearMeshTable();

        void setUpdateUndo(std::function<void()> fn);
        void setRenderContext(const RenderContext::Ptr& renderContext);
        void setRenderTexture(const RenderTexturePtr& renderTexture);

      private: // Methods
        void         handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
        void         handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                             const bool&                        isPressed);
        void         handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
        void         selectMesh(const PhysicsMeshObject::Ptr& meshObject);
        void         unselectMesh(const PhysicsMeshObject::Ptr& meshObject);
        sf::Vector2f getMouseWorldPosition() const;
        //
        bool         handleLeftClickPressOnVertex(const PhysicsMeshObject::Ptr& meshObject);
        bool         handleLeftClickPressOnLine(const PhysicsMeshObject::Ptr& meshObject);
        bool         handleLeftClickPressOnPolygon(const PhysicsMeshObject::Ptr& meshObject);
        bool         handleLeftClickPressOnCircle(const PhysicsMeshObject::Ptr& meshObject);
        bool         handleReleaseLeftClick(const sf::Event::MouseButtonEvent&);
        bool         handleRightClickPressOnVertex(const PhysicsMeshObject::Ptr& meshObject);
        bool         handleRightClickPressOnLine(const PhysicsMeshObject::Ptr& meshObject);
        void         handleLeftClickRelease();
        void         handleRightClickRelease();

      private:
        MeshTable                        m_MeshTable;
        PhysicsMeshObject::Ptr           m_SelectedMesh;
        std::vector<sf::RectangleShape*> m_SelectedVertexTable;
        int                              m_MeshCount;
        RenderTexturePtr                 m_RenderTexture;
        RenderContext::Ptr               m_RenderContext;
        sf::Vector2f                     m_LastMousePosition;
        sf::Vector2f                     m_Epsilon;
        bool                             m_LeftSelection;
        std::function<void()>            m_UpdateUndo;
    };
} // namespace nero
#endif // MESHEDITOR_H
