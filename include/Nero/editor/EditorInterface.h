////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef EDITORINTERFACE_H
#define EDITORINTERFACE_H
///////////////////////////HEADERS//////////////////////////
//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    class EditorInterface
    {

        public:
            typedef std::unique_ptr<EditorInterface> Ptr;

                        EditorInterface(sf::RenderWindow& window);
                       ~EditorInterface();
            void        handleEvent(const sf::Event& event);
            void        update(const sf::Time& timeStep);
            void        render();

            void        updateFrameRate(const float& frameRate, const float& frameTime);

        private:
            void        quitEditor();
            void        createDockSpace();

        private:
            sf::RenderWindow&       m_RenderWindow;
            bool        setup_dock = false;
            ImGuiID actionBarId;
            bool            show_project_window = false;
    };
}

#endif // EDITORINTERFACE_H
