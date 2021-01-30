////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef GRID_H
#define GRID_H
///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/utility/Utility.h>
//SFML
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/CircleShape.hpp>
//STD
#include <deque>
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class Grid : public sf::Transformable, public sf::Drawable
    {
        public:
            typedef std::shared_ptr<Grid>           Ptr;

        public:
                                                    Grid();
            sf::Vector2f                            getCenter();
            void                                    update(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount);
            void                                    updatePosition(const sf::Vector2i& position);
            const sf::Vector2i&                     getTileSize() const;
            const sf::Vector2i&                     getTileCount() const;
            const sf::Vector2i&                     getPosition() const;
            void                                    setColor(const sf::Color& color);
            sf::Color                               getColor();
            nlohmann::json                          toJson();
            void                                    fromJson(nlohmann::json json);

        private:
            void                                    initialize();
            virtual void			                draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            sf::Vector2i                            m_TileSize;
            sf::Vector2i                            m_TileCount;
            sf::Vector2i                            m_Position;
            sf::Color                               m_Color;
            float                                   m_Thickness;
            std::vector<sf::RectangleShape>         m_HorizontalLineTable;
            std::vector<sf::RectangleShape>         m_VerticalLineTable;
    };
}
#endif // GRID_H
