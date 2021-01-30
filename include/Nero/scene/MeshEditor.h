////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef MESHEDITOR_H
#define MESHEDITOR_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/model/Mesh.h>
//STD
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
    class MeshEditor
    {
        public:
            typedef std::shared_ptr<MeshEditor> Ptr;
            typedef sf::RectangleShape          Vertex;
            typedef std::vector<Mesh*>          MeshTab;


                                    MeshEditor(sfg::Canvas::Ptr renderCanvas);
            virtual                ~MeshEditor();

            void                    handleEvent(const sf::Event& event);
            void                    render();

            void                    addMesh(Mesh* mesh);
            void                    deleteMesh(const int& id);
            void                    destroyAllMesh();
            MeshTab&                getMeshTab();

            void                    setUpdateUndo(std::function<void()>  fn);
            void                    setUpdateLog(std::function<void(const std::string&, int)>  fn);
            void                    setUpdateLogIf(std::function<void(const std::string&, bool, int)>  fn);

        private: //Methods
            void                    handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                    handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                    handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);
            void                    rotateMesh(Mesh* mesh, float speed = 0.1f);
            void                    scaleMesh(Mesh* mesh, float scale = 0.1f);
            void                    deselectMesh(Mesh* mesh);

        private:
            MeshTab                                             m_MeshTab;
            Mesh*                                               m_SelectedMesh;
            std::vector<sf::RectangleShape*>                    m_SelectedVertexTab;
            int                                                 m_MeshCount;
            sfg::Canvas::Ptr                                    m_RenderCanvas;
            sf::Vector2f                                        m_LastMousePosition;
            std::function<void()>                               m_UpdateUndo;
            std::function<void(const std::string&, int)>        m_UpdateLog;
            std::function<void(const std::string&, bool, int)>  m_UpdateLogIf;
    };
}
#endif // MESHEDITOR_H
