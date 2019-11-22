////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/model/Grid.h>
////////////////////////////////////////////////////////////
namespace nero
{
    Grid::Grid():
         m_TileSize(sf::Vector2i(50, 50))
        ,m_TileCount(sf::Vector2i(16, 12))
        ,m_Position(sf::Vector2i(0, 0))
        ,m_Thickness(2.f)
        ,m_Color(sf::Color(255, 255, 255, 70))
    {
       initialize();
    }

    void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        for(auto& line : m_HorizontalLineTable)
            target.draw(line, states);

        for(auto& line : m_VerticalLineTable)
            target.draw(line, states);

    }

    void Grid::initialize()
    {
        //create horizontal line
        for(int i= 0; i <= m_TileCount.y; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_TileSize.x*m_TileCount.x, m_Thickness));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(0, i*m_TileSize.y));

            m_HorizontalLineTable.push_back(line);
        }

        //create vertical line
        for(int i=0; i <= m_TileCount.x; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_Thickness, m_TileSize.y*m_TileCount.y));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(i*m_TileSize.x, 0));

            m_VerticalLineTable.push_back(line);
        }
    }

    void Grid::updatePosition(const sf::Vector2i& position)
    {

        sf::Vector2f result;

        result.x = (position.x - m_Position.x) * m_TileSize.x;
        result.y = (position.y - m_Position.y) * m_TileSize.y;

        for(auto& line : m_HorizontalLineTable)
            line.move(result);

        for(auto& line : m_VerticalLineTable)
            line.move(result);

        m_Position = position;
    }

    sf::Vector2f Grid::getCenter()
    {
        sf::Vector2f center;

        center.x = m_HorizontalLineTable.at(0).getPosition().x + m_HorizontalLineTable.at(0).getSize().x/2.f;
        center.y = m_VerticalLineTable.at(0).getPosition().y + m_VerticalLineTable.at(0).getSize().y/2.f;

        return center;
    }

    void Grid::update(const sf::Vector2i& tileSize, const sf::Vector2i& tileCount)
    {
        m_TileSize = tileSize;
        m_TileCount = tileCount;

        m_HorizontalLineTable.clear();
        m_VerticalLineTable.clear();

        initialize();
        updatePosition(m_Position);
    }

    const sf::Vector2i& Grid::getTileSize() const
    {
        return m_TileSize;
    }

    const sf::Vector2i& Grid::getTileCount() const
    {
        return m_TileCount;
    }

    const sf::Vector2i& Grid::getPosition() const
    {
        return m_Position;
    }

    void Grid::setColor(const sf::Color& color)
    {
        m_Color = color;

        initialize();
        update(m_TileSize, m_TileCount);
        updatePosition(m_Position);
    }

    sf::Color Grid::getColor()
    {
        return m_Color;
    }

    nlohmann::json Grid::toJson()
    {
         nlohmann::json json;

        json["tile_size"]   = nero::toJson<sf::Vector2i>(m_TileSize);
        json["tile_count"]  = nero::toJson<sf::Vector2i>(m_TileCount);
        json["position"]    = nero::toJson<sf::Vector2i>(m_Position);
        json["color"]       = nero::toJson(m_Color);

        return json;
    }

    void Grid::fromJson(nlohmann::json json)
    {
        m_TileSize  = vectorFromJson<sf::Vector2i>(json["tile_size"]);
        m_TileCount = vectorFromJson<sf::Vector2i>(json["tile_count"]);
        m_Position  = vectorFromJson<sf::Vector2i>(json["position"]);
        m_Color     = colorFromJson(json["color"]);
    }
}
