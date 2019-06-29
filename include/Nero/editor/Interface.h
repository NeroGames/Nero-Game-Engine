////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef INTERFACE_H
#define INTERFACE_H
///////////////////////////HEADERS//////////////////////////
//IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui-SFML.h>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

////////////////////////////////////////////////////////////

namespace nero
{
    class Interface
    {

        public:
            typedef std::unique_ptr<Interface> Ptr;

                        Interface(sf::RenderWindow& window);
                       ~Interface();
            void        handleEvent(const sf::Event& event);
            void        update(const sf::Time& timeStep);
            void        render();

            void        updateFrameRate(const float& frameRate, const float& frameTime);

        private:
            void        quitEditor();
            void        createDockSpace();

            void        showProjectManagerWindow();

        private:
            sf::RenderWindow&       m_RenderWindow;
            bool        setup_dock = false;
            ImGuiID actionBarId;
            bool            show_project_window = false;
            sf::Texture     project_manager_texture;

    };
}

#endif // INTERFACE_H
