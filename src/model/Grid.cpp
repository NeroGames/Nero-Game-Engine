////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <NERO/model/Grid.h>
////////////////////////////////////////////////////////////
namespace nero
{
    Grid::Grid(sf::Vector2f gridSize):
        m_Size(gridSize)
        ,m_Color(transparent_color(sf::Color::White, 70))
        ,m_xOffset(50.f)
        ,m_yOffset(50.f)
        ,m_LastPosition(sf::Vector2f(0.f, 0.f))
        ,m_LastRotation(0.f)
        ,m_Position(sf::Vector2f(0.f, 0.f))
        ,m_Rotation(0.f)
        ,m_Thickness(-2.f)
        ,m_Origin(sf::Vector2f(gridSize.x/2.f, gridSize.y/2.f))
        ,m_CountOffset(4)
    {
       initialize();
    }

    void Grid::update(sf::View view)
    {
        if(m_LastRotation != view.getRotation())
        {
            //updateRotation(view);
            //m_LastRotation = view.getRotation();
        }

        if(m_LastPosition != view.getCenter())
        {
            updatePosition(view);
            m_LastPosition = view.getCenter();
        }
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
        //get number of lines
        int xCount = std::ceil(m_Size.x / m_xOffset);
        int yCount = std::ceil(m_Size.y / m_yOffset);

        xCount >= yCount ? yCount = xCount : xCount = yCount;

        xCount += m_CountOffset;
        yCount += m_CountOffset;

        //create horizontal line
        for(int i=-m_CountOffset; i < yCount; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_xOffset*xCount, m_Thickness));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(-m_CountOffset*m_yOffset, i*m_yOffset));

            m_HorizontalLineTable.push_back(line);
        }

        //create vertical line
        for(int i=-m_CountOffset; i < xCount; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_Thickness, m_yOffset*yCount));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(i*m_xOffset, -m_CountOffset*m_xOffset));

            m_VerticalLineTable.push_back(line);
        }
    }

    void Grid::initialize_test()
    {
        int xCount = 6;
        int yCount = 6;

        //create horizontal line
        for(int i=2; i <7 ; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_xOffset*4, m_Thickness));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(2*m_yOffset, i*m_xOffset));

            m_HorizontalLineTable.push_back(line);
        }

        //create vertical line
        for(int i=2; i < 7; i++)
        {
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(m_Thickness, m_yOffset*4));
            line.setFillColor(m_Color);
            line.setPosition(sf::Vector2f(i*m_xOffset, 2*m_xOffset));

            m_VerticalLineTable.push_back(line);
        }
    }

    void Grid::updateRotation(sf::View view)
    {
        float angle =  m_LastRotation - view.getRotation();

        for(auto& line : m_HorizontalLineTable)
        {
            sf::Vector2f point1 = line.getPosition();
            point1 = rotateVertex(m_Origin, angle, point1);
            line.setPosition(point1);
            line.rotate(angle);
        }

        for(auto& line : m_VerticalLineTable)
        {
            sf::Vector2f point1 = line.getPosition();
            point1 = rotateVertex(m_Origin, angle, point1);
            line.setPosition(point1);
            line.rotate(angle);
        }

        m_Rotation += angle;
    }

    void Grid::updatePosition(sf::View view)
    {
        sf::Vector2f offset = view.getCenter() - m_LastPosition;

        m_Position += offset;
        sf::Transform t;
        t.rotate(-view.getRotation());
        sf::Vector2f result = t * -offset;

        for(auto& line : m_HorizontalLineTable)
            line.move(result);

        for(auto& line : m_VerticalLineTable)
            line.move(result);


        if(m_Position.x < -m_xOffset && offset.x < 0 )
        {
            handleRightOffset(view);
        }
        else if(m_Position.x > m_xOffset && offset.x > 0 )
        {
             handleLeftOffset(view);
        }
        if(m_Position.y > m_yOffset && offset.y > 0 )
        {
            handleUpOffset(view);
        }
        else if(m_Position.y < -m_yOffset && offset.y < 0 )
        {
            handleDownOffset(view);
        }
    }

    void Grid::handleRightOffset(sf::View view)
    {
        GridFace face = getRightFace(m_Rotation);

        if(face == GridFace::V_BACK && axeAlign())
        {
            m_VerticalLineTable.pop_back();
            auto line = m_VerticalLineTable.front();
            line.move(sf::Vector2f(-m_xOffset, 0.f));
            m_VerticalLineTable.push_front(line);

            for(auto& line : m_HorizontalLineTable)
                line.move(sf::Vector2f(-m_xOffset, 0.f));

            m_Position.x += m_xOffset;
        }

//        else if(face == GridFace::V_BACK && !axeAlign())
//        {
//            m_VerticalLineTable.pop_back();
//
//            auto line1 = m_VerticalLineTable.front();
//            line1.move(sf::Vector2f(-m_xOffset/std::cos(toRadian(m_Rotation)), 0.f));
//            m_VerticalLineTable.push_front(line1);
//
//            for(auto& line : m_HorizontalLineTable)
//                line.move(translateLine(sf::Vector2f(-m_xOffset, 0.f), -view.getRotation()));
//
//            auto line2= m_HorizontalLineTable.back();
//            line2.move(translateLine(sf::Vector2f(0.f, m_xOffset), -view.getRotation()));
//
//            auto pos1 = line1.getPosition();
//            auto pos2 = m_HorizontalLineTable.front().getPosition();
//
//            if(distance(pos1, pos2) > m_xOffset)
//            {
//                 m_HorizontalLineTable.pop_front();
//                 m_HorizontalLineTable.push_back(line2);
//            }
//
//            m_Position.x += m_xOffset;
//        }
//
//        else if(face == GridFace::H_BACK && !axeAlign())
//        {
//            m_VerticalLineTable.pop_back();
//
//            for(auto& line : m_HorizontalLineTable)
//                line.move(translateLine(sf::Vector2f(-m_xOffset, 0.f), -view.getRotation()));
//
//            auto line1 = m_VerticalLineTable.front();
//            line1.move(sf::Vector2f(-m_xOffset/std::cos(toRadian(m_Rotation)), 0.f));
//
//            m_VerticalLineTable.push_front(line1);
//
//            auto line2= m_HorizontalLineTable.front();
//            line2.move(translateLine(sf::Vector2f(0.f, -m_xOffset), -view.getRotation()));
//
//            auto pos1 = line1.getPosition();
//            auto pos2 = m_HorizontalLineTable.back().getPosition();
//
//            if(distance(pos1, pos2) > line1.getSize().y/std::cos(toRadian(m_Rotation)))
//            {
//                 m_HorizontalLineTable.pop_back();
//                 m_HorizontalLineTable.push_front(line2);
//            }
//
//            m_Position.x += m_xOffset;
//        }
    }


    void Grid::handleLeftOffset(sf::View view)
    {
        GridFace face = getLeftFace(m_Rotation);

        if(face == GridFace::V_FRONT && axeAlign())
        {
            m_VerticalLineTable.pop_front();
            auto line = m_VerticalLineTable.back();
            line.move(sf::Vector2f(m_xOffset, 0.f));
            m_VerticalLineTable.push_back(line);

            for(auto& line : m_HorizontalLineTable)
                line.move(sf::Vector2f(m_xOffset, 0.f));

            m_Position.x -= m_xOffset;
        }
    }

    void Grid::handleUpOffset(sf::View view)
    {
        GridFace face = getUpFace(m_Rotation);

        if(face == GridFace::H_FRONT && axeAlign())
        {
            m_HorizontalLineTable.pop_front();
            auto line = m_HorizontalLineTable.back();
            line.move(sf::Vector2f(0.f, m_xOffset));
            m_HorizontalLineTable.push_back(line);

            for(auto& line : m_VerticalLineTable)
                line.move(sf::Vector2f(0.f, m_xOffset));

            m_Position.y -= m_yOffset;
        }
    }

    void Grid::handleDownOffset(sf::View view)
    {
        GridFace face = getDownFace(m_Rotation);

        if(face == GridFace::H_BACK && axeAlign())
        {
            m_HorizontalLineTable.pop_back();
            auto line = m_HorizontalLineTable.front();
            line.move(sf::Vector2f(0.f, -m_xOffset));
            m_HorizontalLineTable.push_front(line);

            for(auto& line : m_VerticalLineTable)
                line.move(sf::Vector2f(0.f, -m_xOffset));

            m_Position.y += m_yOffset;
        }
    }

    sf::Vector2f Grid::translateLine(sf::Vector2f offset, float angle)
    {
        sf::Transform t;
        t.rotate(angle);

        return t * offset;
    }

    bool Grid::axeAlign()
    {
        return m_Rotation == 0.f ||  m_Rotation == 90.f || m_Rotation == 180.f || m_Rotation == 270.f || m_Rotation == -90.f || m_Rotation == -180.f || m_Rotation == -270.f;
    }

    GridFace Grid::getRightFace(float angle)
    {
         if(angle >= -90.f && angle < 0.f)
            return GridFace::H_BACK;
        else if(angle >= -180.f && angle < -90.f)
            return GridFace::V_FRONT;
        else if(angle >= -270.f && angle < -180.f)
            return GridFace::H_FRONT;
        else if(angle >= -360.f && angle < -270.f)
            return GridFace::V_BACK;
        else if(angle > 0.f && angle <= 90.f)
            return GridFace::H_FRONT;
        else if(angle > 90.f && angle <= 180.f)
            return GridFace::V_FRONT;
        else if(angle > 180.f && angle <= 270.f)
            return GridFace::H_BACK;
        else if(angle > 270 && angle <= 360.f)
            return GridFace::V_BACK ;
        else
            return GridFace::V_BACK;
    }

    GridFace Grid::getLeftFace(float angle)
    {
        GridFace rightFace = getRightFace(angle);

        if(rightFace == GridFace::V_BACK)
            return GridFace::V_FRONT;
        else if(rightFace == GridFace::V_FRONT)
             return GridFace::V_BACK;
        else if(rightFace == GridFace::H_FRONT)
             return GridFace::H_BACK;
        else if(rightFace == GridFace::H_BACK)
             return GridFace::H_FRONT;
    }

    GridFace Grid::getUpFace(float angle)
    {
        GridFace rightFace = getRightFace(angle);

        if(rightFace == GridFace::V_BACK)
            return GridFace::H_FRONT;
        else if(rightFace == GridFace::V_FRONT)
             return GridFace::H_BACK;
        else if(rightFace == GridFace::H_FRONT)
             return GridFace::V_FRONT;
        else if(rightFace == GridFace::H_BACK)
             return GridFace::V_BACK;
    }

    GridFace Grid::getDownFace(float angle)
    {
        GridFace rightFace = getRightFace(angle);

        if(rightFace == GridFace::V_BACK)
            return GridFace::H_BACK;
        else if(rightFace == GridFace::V_FRONT)
             return GridFace::H_FRONT;
        else if(rightFace == GridFace::H_FRONT)
             return GridFace::V_BACK;
        else if(rightFace == GridFace::H_BACK)
             return GridFace::V_FRONT;
    }
}
