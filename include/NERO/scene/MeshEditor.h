////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
// All rights reserved
//
////////////////////////////////////////////////////////////

#ifndef MESHEDITOR_H
#define MESHEDITOR_H

///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/object/Mesh.h>
//STD
#include <vector>
////////////////////////////////////////////////////////////

namespace nero
{
    class MeshEditor
    {
        public:
            typedef sf::RectangleShape  Vertex;
            typedef std::vector<Mesh*>  MeshTab;


                                    MeshEditor(sfg::Canvas::Ptr& renderCanvas);
            virtual                ~MeshEditor();

            void                    handleEvent(const sf::Event& event);
            void                    render();


            void                    addMesh(Mesh* mesh);
            void                    deleteMesh(const int& id);
            void                    destroyAllMesh();
            MeshTab&                getMeshTab();

            void                    setEngineUndoFunction(std::function<void()>  f);


        private: //Methods
            void                    handleKeyboardInput(const sf::Keyboard::Key& key, const bool& isPressed);
            void                    handleMouseButtonsInput(const sf::Event::MouseButtonEvent& mouse, const bool& isPressed);
            void                    handleMouseMoveInput(const sf::Event::MouseMoveEvent& mouse);

            void                    rotateMesh(Mesh* mesh, float speed = 0.1f);
            void                    scaleMesh(Mesh* mesh, float scale = 0.1f);

            void                    deselectMesh(Mesh* mesh);

        private:
            MeshTab                                 m_MeshTab;
            Mesh*                                   m_SelectedMesh;
            std::vector<sf::RectangleShape*>        m_SelectedVertexTab;

            int                                     m_MeshCount;

            sfg::Canvas::Ptr&                       m_RenderCanvas;
            sf::Vector2f                            m_LastMousePosition;
            std::function<void()>                   m_EngineUndo;
    };
}

#endif // MESHEDITOR_H
