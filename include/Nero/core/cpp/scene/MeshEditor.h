////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MESHEDITOR_H
#define MESHEDITOR_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/object/PhysicalMeshObject.h>
#include <Nero/core/cpp/scene/SceneUtility.h>
#include <SFML/Graphics/RenderTexture.hpp>
// STD
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class MeshEditor
    {
      public:
        typedef std::shared_ptr<MeshEditor>          Ptr;
        typedef std::shared_ptr<sf::RenderTexture>   RenderTexturePtr;
        typedef sf::RectangleShape                   Vertex;
        typedef std::vector<PhysicalMeshObject::Ptr> MeshTab;

        MeshEditor();
        virtual ~MeshEditor();

        void     handleEvent(const sf::Event& event);

        void     addMesh(PhysicalMeshObject::Ptr mesh);
        void     deleteMesh(const int& id);
        void     destroyAllMesh();
        MeshTab& getMeshTab();

        void     setUpdateUndo(std::function<void()> fn);
        void     setUpdateLog(std::function<void(const std::string&, int)> fn);
        void     setUpdateLogIf(std::function<void(const std::string&, bool, int)> fn);
        void     setRenderContext(const RenderContext::Ptr& renderContext);
        void     setRenderTexture(const RenderTexturePtr& renderTexture);

      private: // Methods
        void handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
        void handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse,
                                     const bool&                        isPressed);
        void handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
        void rotateMesh(PointMesh::Ptr mesh, float speed = 0.1f);
        void scaleMesh(PointMesh::Ptr mesh, float scale = 0.1f);
        void deselectMesh(PhysicalMeshObject::Ptr meshObject);

      private:
        MeshTab                                            m_MeshTab;
        PhysicalMeshObject::Ptr                            m_SelectedMesh;
        std::vector<sf::RectangleShape*>                   m_SelectedVertexTab;
        int                                                m_MeshCount;
        RenderTexturePtr                                   m_RenderTexture;
        RenderContext::Ptr                                 m_RenderContext;
        sf::Vector2f                                       m_LastMousePosition;
        std::function<void()>                              m_UpdateUndo;
        std::function<void(const std::string&, int)>       m_UpdateLog;
        std::function<void(const std::string&, bool, int)> m_UpdateLogIf;
    };
} // namespace nero
#endif // MESHEDITOR_H
