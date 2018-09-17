////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
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
                                                    Grid(sf::Vector2f gridSize);
            void                                    update(sf::View view);

        private:

            void                                    initialize();
            void                                    initialize_test();
            virtual void			                draw(sf::RenderTarget& target, sf::RenderStates states) const;
            void                                    updateRotation(sf::View view);
            void                                    updatePosition(sf::View view);
            GridFace                                getRightFace(float angle);
            GridFace                                getLeftFace(float angle);
            GridFace                                getUpFace(float angle);
            GridFace                                getDownFace(float angle);
            void                                    handleRightOffset(sf::View view);
            void                                    handleLeftOffset(sf::View view);
            void                                    handleUpOffset(sf::View view);
            void                                    handleDownOffset(sf::View view);
            bool                                    axeAlign();
            sf::Vector2f                            translateLine(sf::Vector2f offset, float angle);



        private:
            sf::Vector2f                            m_Size;
            sf::Vector2f                            m_Origin;
            sf::Vector2f                            m_Position;
            float                                   m_Rotation;
            sf::Vector2f                            m_LastPosition;
            float                                   m_LastRotation;
            sf::Color                               m_Color;
            float                                   m_xOffset;
            float                                   m_yOffset;
            int                                     m_CountOffset;
            float                                   m_Thickness;
            std::deque<sf::RectangleShape>          m_HorizontalLineTable;
            std::deque<sf::RectangleShape>          m_VerticalLineTable;
    };
}

#endif // GRID_H
